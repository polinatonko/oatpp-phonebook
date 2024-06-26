#ifndef TEST_COMPONENT_HPP
#define TEST_COMPONENT_HPP

#include "oatpp/web/client/HttpRequestExecutor.hpp"
#include "oatpp/web/server/AsyncHttpConnectionHandler.hpp"

#include "oatpp/network/virtual_/client/ConnectionProvider.hpp"
#include "oatpp/network/virtual_/server/ConnectionProvider.hpp"
#include "oatpp/network/virtual_/Interface.hpp"

#include "oatpp/network/tcp/server/ConnectionProvider.hpp"
#include "oatpp/network/tcp/client/ConnectionProvider.hpp"

#include "oatpp/json/ObjectMapper.hpp"
#include "oatpp/macro/component.hpp"

#include "TestClient.hpp"
#include "../../src/DbComponent.hpp"
#include "../../src/SwaggerComponent.hpp"
#include "../../src/errorHandler/ErrorHandler.hpp"

namespace {

  /**
   * Class which configures test components.
   */
  class TestComponent {
    v_uint16 m_port;

  public:
    /**
     * Constructor with port number.
     * @param port - port number.
     */
    TestComponent(v_uint16 port)
      : m_port(port)
    {}

    /**
     * Create db component
     */
    OATPP_CREATE_COMPONENT(std::shared_ptr<EntryDb>, entryDb) ([] {
      auto connectionProvider = std::make_shared<oatpp::sqlite::ConnectionProvider>(DATABASE_FILE);
      auto executor = std::make_shared<oatpp::sqlite::Executor>(connectionProvider);

      return std::make_shared<EntryDb>(executor);
    } ());

    /**
     * Create oatpp virtual network interface for test networking
     */
    OATPP_CREATE_COMPONENT(std::shared_ptr<oatpp::network::virtual_::Interface>, virtualInterface)([] {
      return oatpp::network::virtual_::Interface::obtainShared("virtualhost");
    }());

    /**
     * Create server ConnectionProvider for test
     */
    OATPP_CREATE_COMPONENT(std::shared_ptr<oatpp::network::ServerConnectionProvider>, serverConnectionProvider)([this] {
      if(m_port == 0) {
        OATPP_COMPONENT(std::shared_ptr<oatpp::network::virtual_::Interface>, _interface);
        return std::static_pointer_cast<oatpp::network::ServerConnectionProvider>(
          oatpp::network::virtual_::server::ConnectionProvider::createShared(_interface)
        );
      }

      return std::static_pointer_cast<oatpp::network::ServerConnectionProvider>(
        oatpp::network::tcp::server::ConnectionProvider::createShared({"localhost", m_port})
      );

    }());

    /**
      * Create client ConnectionProvider for test
      */
    OATPP_CREATE_COMPONENT(std::shared_ptr<oatpp::network::ClientConnectionProvider>, clientConnectionProvider)([this] {

      if(m_port == 0) {
        OATPP_COMPONENT(std::shared_ptr<oatpp::network::virtual_::Interface>, _interface);
        return std::static_pointer_cast<oatpp::network::ClientConnectionProvider>(
          oatpp::network::virtual_::client::ConnectionProvider::createShared(_interface)
        );
      }

      return std::static_pointer_cast<oatpp::network::ClientConnectionProvider>(
        oatpp::network::tcp::client::ConnectionProvider::createShared({"localhost", m_port})
      );

    }());

    /**
      *  Create Router component
      */
    OATPP_CREATE_COMPONENT(std::shared_ptr<oatpp::web::server::HttpRouter>, httpRouter)([] {
      return oatpp::web::server::HttpRouter::createShared();
    }());

    /**
      * Create Async Executor
      */
    OATPP_CREATE_COMPONENT(std::shared_ptr<oatpp::async::Executor>, executor)([] {
      return std::make_shared<oatpp::async::Executor>(
        4 /* Data-Processing threads */,
        1 /* I/O threads */,
        1 /* Timer threads */
      );
    }());

    /**
     *  Create ObjectMapper component to serialize/deserialize DTOs in Contoller's API
     */
    OATPP_CREATE_COMPONENT(std::shared_ptr<oatpp::web::mime::ContentMappers>, apiContentMappers)([] {
      auto json = std::make_shared<oatpp::json::ObjectMapper>();
      json->serializerConfig().json.useBeautifier = true;
      json->deserializerConfig().mapper.allowUnknownFields = false;

      auto mappers = std::make_shared<oatpp::web::mime::ContentMappers>();
      mappers->putMapper(json);

      return mappers;

    }());

    /**
     *  Create ConnectionHandler component which uses Router component to route requests
     */
    OATPP_CREATE_COMPONENT(std::shared_ptr<oatpp::network::ConnectionHandler>, serverConnectionHandler)([] {
      OATPP_COMPONENT(std::shared_ptr<oatpp::web::server::HttpRouter>, router); // get Router component
      OATPP_COMPONENT(std::shared_ptr<oatpp::async::Executor>, executor); // get Executor component
      OATPP_COMPONENT(std::shared_ptr<oatpp::web::mime::ContentMappers>, apiContentMappers); // get ContentMappers component

      auto connectionHandler = oatpp::web::server::AsyncHttpConnectionHandler::createShared(router, executor);
      connectionHandler->setErrorHandler(std::make_shared<ErrorHandler>(
        apiContentMappers->getMapper("application/json")
      ));

      return connectionHandler;
    }());


    OATPP_CREATE_COMPONENT(std::shared_ptr<TestClient>, client)([] {

      OATPP_COMPONENT(std::shared_ptr<oatpp::network::ClientConnectionProvider>, clientConnectionProvidr);
      OATPP_COMPONENT(std::shared_ptr<oatpp::web::mime::ContentMappers>, apiContentMappers);

      auto retryPolicy = std::make_shared<oatpp::web::client::SimpleRetryPolicy>(5, std::chrono::seconds(1));
      auto requestExecutor = oatpp::web::client::HttpRequestExecutor::createShared(clientConnectionProvidr, retryPolicy);
      
      return TestClient::createShared(requestExecutor, apiContentMappers->getMapper("application/json"));
    }());

  };

}

#endif /* TEST_COMPONENT_HPP */