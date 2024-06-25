#include "./controller/MyController.hpp"
#include "./AppComponent.hpp"
#include "./DbComponent.hpp"

#include "oatpp/network/Server.hpp"
#include "oatpp-swagger/AsyncController.hpp"

#include <iostream>

void run() {

  /* Register Components in scope of run() method */
  AppComponent components;

  /* Get router component */
  OATPP_COMPONENT(std::shared_ptr<oatpp::web::server::HttpRouter>, router);

  /* Create controllers and generate API docs */
  oatpp::web::server::api::Endpoints docEndpoints;
  docEndpoints.append(router->addController(std::make_shared<MyController>())->getEndpoints());

  router->addController(oatpp::swagger::AsyncController::createShared(docEndpoints));
  
  /* Get connection handler component */
  OATPP_COMPONENT(std::shared_ptr<oatpp::network::ConnectionHandler>, connectionHandler);

  /* Get connection provider component */
  OATPP_COMPONENT(std::shared_ptr<oatpp::network::ServerConnectionProvider>, connectionProvider);

  /* Create server which takes provided TCP connections and passes them to HTTP connection handler */
  oatpp::network::Server server(connectionProvider, connectionHandler);

  /* Print info about server port */
  OATPP_LOGi("MyApp", "Server running on port {}", connectionProvider->getProperty("port").toString());

  /* Run server */
  server.run();

  /* stop db connection pool */
  OATPP_COMPONENT(std::shared_ptr<oatpp::provider::Provider<oatpp::sqlite::Connection>>, dbConnectionProvider);
  dbConnectionProvider->stop();
  
}

/**
 *  main
 */
int main(int argc, const char * argv[]) {

  oatpp::Environment::init();

  run();
  
  /* Print how much objects were created during app running, and what have left-probably leaked */
  /* Disable object counting for release builds using '-D OATPP_DISABLE_ENV_OBJECT_COUNTERS' flag for better performance */
  std::cout << "\nEnvironment:\n";
  std::cout << "objectsCount = " << oatpp::Environment::getObjectsCount() << "\n";
  std::cout << "objectsCreated = " << oatpp::Environment::getObjectsCreated() << "\n\n";
  
  oatpp::Environment::destroy();
  
  return 0;
}
