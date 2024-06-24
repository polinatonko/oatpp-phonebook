#ifndef APP_COMPONENT_HPP
#define APP_COMPONENT_HPP

#include "oatpp/web/server/AsyncHttpConnectionHandler.hpp"
#include "oatpp/web/mime/ContentMappers.hpp"
#include "oatpp/network/tcp/server/ConnectionProvider.hpp"
#include "oatpp/json/ObjectMapper.hpp"
#include "oatpp/macro/component.hpp"

#include "DbComponent.hpp"
#include "SwaggerComponent.hpp"
#include "errorHandler/ErrorHandler.hpp"

/**
 *  Class which creates and holds Application components and registers components in oatpp::base::Environment
 *  Order of components initialization is from top to bottom
 */
class AppComponent {
public:
  /**
   * Db component
   */
  DbComponent dbComponent;

  /**
   * Swagger component
   */
  SwaggerComponent swaggerComponent;

  /**
   *  Create ConnectionProvider component which listens on the port
   */
  OATPP_CREATE_COMPONENT(std::shared_ptr<oatpp::network::ServerConnectionProvider>, serverConnectionProvider)([] {
    return oatpp::network::tcp::server::ConnectionProvider::createShared({"0.0.0.0", 8000, oatpp::network::Address::IP_4});
  }());
  
  /**
   *  Create Router component
   */
  OATPP_CREATE_COMPONENT(std::shared_ptr<oatpp::web::server::HttpRouter>, httpRouter)([] {
    return oatpp::web::server::HttpRouter::createShared();
  }());

  OATPP_CREATE_COMPONENT(std::shared_ptr<oatpp::web::mime::ContentMappers>, apiContentMappers)([] {

    auto json = std::make_shared<oatpp::json::ObjectMapper>();
    json->serializerConfig().json.useBeautifier = true;
    json->deserializerConfig().mapper.allowUnknownFields = false;

    auto mappers = std::make_shared<oatpp::web::mime::ContentMappers>();
    mappers->putMapper(json);

    return mappers;

  }());
  
  /**
   *  Create ConnectionHandler component which uses Router component to route requests;
   *  Set custom ErrorHandler
   */
  OATPP_CREATE_COMPONENT(std::shared_ptr<oatpp::network::ConnectionHandler>, serverConnectionHandler)([] {
    OATPP_COMPONENT(std::shared_ptr<oatpp::web::server::HttpRouter>, router); // get Router component
    OATPP_COMPONENT(std::shared_ptr<oatpp::web::mime::ContentMappers>, apiContentMappers); // get ContentMappers component

    auto connectionHandler = oatpp::web::server::AsyncHttpConnectionHandler::createShared(router);
    connectionHandler->setErrorHandler(std::make_shared<ErrorHandler>(
      apiContentMappers->getMapper("application/json")
    ));

    return connectionHandler;
  }());

};

#endif /* APP_COMPONENT_HPP */
