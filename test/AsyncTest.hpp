
#ifndef ASYNC_TEST_HPP
#define ASYNC_TEST_HPP

#include "oatpp/base/Log.hpp"

#include "oatpp/macro/component.hpp"

#include "oatpp/web/client/HttpRequestExecutor.hpp"

#include "oatpp/web/server/AsyncHttpConnectionHandler.hpp"
#include "oatpp/web/server/HttpRouter.hpp"

#include "oatpp/json/ObjectMapper.hpp"

#include "oatpp/network/tcp/server/ConnectionProvider.hpp"
#include "oatpp/network/tcp/client/ConnectionProvider.hpp"

#include "oatpp/network/virtual_/client/ConnectionProvider.hpp"
#include "oatpp/network/virtual_/server/ConnectionProvider.hpp"
#include "oatpp/network/virtual_/Interface.hpp"

#include "oatpp-test/web/ClientServerTestRunner.hpp"
#include "oatpp-test/UnitTest.hpp"

#include "app/TestClient.hpp"
#include "app/TestComponent.hpp"

#include "GetEntryByIdCoroutine.hpp"
#include "GetEntriesCoroutine.hpp"
#include "UpdateEntryCoroutine.hpp"
#include "DeleteEntryCoroutine.hpp"
#include "CreateEntryCoroutine.hpp"

#include "../src/controller/MyController.hpp"

namespace {

/**
 * Class which performs testing async requests.
 */
class AsyncTest : public oatpp::test::UnitTest {
  v_uint16 m_port;
  v_int32 m_connectionsPerEndpoint;

public:
  /**
   * Constructor with port number and number of connections per point.
   * @param port - server port number.
   * @param connectionsPerEndpoint - number of connections per point.
   */
  AsyncTest(v_uint16 port, v_int32 connectionsPerEndpoint)
    : UnitTest("TEST[AsyncTest]")
    , m_port(port)
    , m_connectionsPerEndpoint(connectionsPerEndpoint)
  {}

  void onRun() override {
    TestComponent component(m_port);

    oatpp::test::web::ClientServerTestRunner runner;

    OATPP_COMPONENT(std::shared_ptr<oatpp::web::mime::ContentMappers>, apiContentMappers);
    auto controller = std::make_shared<MyController>(apiContentMappers);
    runner.addController(controller);

    runner.run([this] {
      OATPP_COMPONENT(std::shared_ptr<oatpp::async::Executor>, executor);

      GetEntryByIdCoroutine::SUCCESS_COUNTER = 0;
      GetEntriesCoroutine::SUCCESS_COUNTER = 0;

      OATPP_LOGv("Client", "Iterations={}", m_connectionsPerEndpoint)

      for(v_int32 i = 0; i < m_connectionsPerEndpoint; ++i) {
        executor->execute<GetEntryByIdCoroutine>();
        executor->execute<GetEntriesCoroutine>();
      }

      while (GetEntryByIdCoroutine::SUCCESS_COUNTER != -1 || GetEntriesCoroutine::SUCCESS_COUNTER != -1) {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));

        if(GetEntryByIdCoroutine::SUCCESS_COUNTER == m_connectionsPerEndpoint) {
          GetEntryByIdCoroutine::SUCCESS_COUNTER = -1;
          OATPP_LOGv("Client", "getEntryById - DONE!")
        }

        if (GetEntriesCoroutine::SUCCESS_COUNTER == m_connectionsPerEndpoint) {
          GetEntriesCoroutine::SUCCESS_COUNTER = -1;
          OATPP_LOGv("Client", "getEntries - DONE!")
        }
      }
    
      /* -1 is success */
      OATPP_ASSERT(GetEntryByIdCoroutine::SUCCESS_COUNTER == -1)
      OATPP_ASSERT(GetEntriesCoroutine::SUCCESS_COUNTER == -1)

      /* Wait executor tasks before quit */
      executor->waitTasksFinished();
      executor->stop();

    }, std::chrono::minutes(10));

    OATPP_COMPONENT(std::shared_ptr<oatpp::async::Executor>, executor);
    executor->join();
  }

};

}

#endif /* ASYNC_TEST_HPP */