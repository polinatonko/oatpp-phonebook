
#include <iostream>

#include "AsyncTest.hpp"
#include "LogicTest.hpp"

void runTests() {
  {
    LogicTest test(8000);
    test.run(1);
  }
  
  {
    AsyncTest test_virtual(0, 10);
    test_virtual.run(1);

    AsyncTest test_port(8000, 10);
    test_port.run(1);
  }
}

int main() {

  oatpp::Environment::init();

  runTests();

  /* Print how much objects were created during app running, and what have left-probably leaked */
  std::cout << "\nEnvironment:\n";
  std::cout << "objectsCount = " << oatpp::Environment::getObjectsCount() << "\n";
  std::cout << "objectsCreated = " << oatpp::Environment::getObjectsCreated() << "\n\n";

  OATPP_ASSERT(oatpp::Environment::getObjectsCount() == 0);

  oatpp::Environment::destroy();

  return 0;
}
