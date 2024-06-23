cmake_minimum_required(VERSION 3.20)

set(project_name phonebook)

project(${project_name})

set(CMAKE_CXX_STANDARD 17)

add_library(${project_name}-lib
        src/AppComponent.hpp
        src/DbComponent.hpp
        src/SwaggerComponent.hpp
        src/controller/MyController.hpp
        src/dto/DTOs.hpp
        src/service/EntryService.hpp
        src/service/EntryService.cpp
        src/db/EntryDb.hpp
)

## link libs

find_package(Threads REQUIRED)
find_package(oatpp 1.4.0 REQUIRED)
find_package(oatpp-sqlite 1.4.0 REQUIRED)
find_package(oatpp-swagger 1.4.0 REQUIRED)

target_link_libraries(${project_name}-lib
        PUBLIC oatpp::oatpp
        PUBLIC oatpp::oatpp-test
        PUBLIC oatpp::oatpp-sqlite
        PUBLIC oatpp::oatpp-swagger
)

target_include_directories(${project_name}-lib PUBLIC src)

add_definitions(
        ## define path to swagger-ui static resources folder
        -DOATPP_SWAGGER_RES_PATH="${oatpp-swagger_INCLUDE_DIRS}/../bin/oatpp-swagger/res"

        ## SQLite database file
        -DDATABASE_FILE="${CMAKE_CURRENT_SOURCE_DIR}/db.sqlite"
        
        ## SQLite database test file
        -DTESTDATABASE_FILE="${CMAKE_CURRENT_SOURCE_DIR}/test-db.sqlite"

        ## Path to database migration scripts
        -DDATABASE_MIGRATIONS="${CMAKE_CURRENT_SOURCE_DIR}/sql"
)

## add executables

add_executable(${project_name}-exe
        src/App.cpp
        test/app/MyApiTestClient.hpp)
target_link_libraries(${project_name}-exe ${project_name}-lib)
add_dependencies(${project_name}-exe ${project_name}-lib)

add_executable(${project_name}-test
        test/tests.cpp
        test/app/TestComponent.hpp
        test/app/MyApiTestClient.hpp
        test/MyControllerTest.cpp
        test/MyControllerTest.hpp
)

target_link_libraries(${project_name}-test ${project_name}-lib)
add_dependencies(${project_name}-test ${project_name}-lib)

set_target_properties(${project_name}-lib ${project_name}-exe ${project_name}-test PROPERTIES
        CXX_STANDARD 17
        CXX_EXTENSIONS OFF
        CXX_STANDARD_REQUIRED ON
)

enable_testing()
add_test(project-tests ${project_name}-test)
