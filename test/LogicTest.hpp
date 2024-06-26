#ifndef LOGIC_TEST_HPP
#define LOGIC_TEST_HPP

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
 * Class which performs testing all types of requests.
 */
class LogicTest : public oatpp::test::UnitTest {
  v_uint16 m_port;

public:

  /**
   * Constructor with port number and number.
   * @param port - server port number.
   */
  LogicTest(v_uint16 port)
    : UnitTest("TEST[LogicTest]")
    , m_port(port)
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
      CreateEntryCoroutine::SUCCESS_COUNTER = 0;
      DeleteEntryCoroutine::SUCCESS_COUNTER = 0;
      UpdateEntryCoroutine::SUCCESS_COUNTER = 0;

      executor->execute<CreateEntryCoroutine>();

      while(CreateEntryCoroutine::SUCCESS_COUNTER != 1) {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
      }

      executor->execute<UpdateEntryCoroutine>();

      while(UpdateEntryCoroutine::SUCCESS_COUNTER != 1) {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
      }

      executor->execute<DeleteEntryCoroutine>();

      while(DeleteEntryCoroutine::SUCCESS_COUNTER != 1) {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
      }

      executor->execute<GetEntryByIdCoroutine>();

      while(GetEntryByIdCoroutine::SUCCESS_COUNTER != 1) {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
      }

      executor->execute<GetEntriesCoroutine>();

      while(GetEntriesCoroutine::SUCCESS_COUNTER != 1) {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
      }
    
      /* Wait executor tasks before quit */
      executor->waitTasksFinished();
      executor->stop();

    }, std::chrono::minutes(10));

    OATPP_COMPONENT(std::shared_ptr<oatpp::async::Executor>, executor);
    executor->join();
  }

};

}

#endif /* LOGIC_TEST_HPP */