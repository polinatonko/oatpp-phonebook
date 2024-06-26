# oatpp-phonebook

Phonebook API built with Oat++, using oatpp ORM with SQLite and documented with Swagger and OpenApi 3.0.0.

## Overview

This project is using the following oatpp modules:

- [oatpp](https://github.com/oatpp/oatpp) 
- [oatpp-swagger](https://github.com/oatpp/oatpp-swagger)
- [oatpp-sqlite](https://github.com/oatpp/oatpp-sqlite)

### Project layout

```
|- CMakeLists.txt                        // Projects CMakeLists.txt
|- config.json                           // Configuration file
|- src/
|    |- config/                          // Folder containing Configuration where config.json is parsed
|    |- controller/                      // Folder containing MyController where all endpoints are declared
|    |- db/                              // Folder containing the database client
|    |- dto/                             // DTOs and Validators
|    |- errorHandler/                    // Error handler and exceptions
|    |- service/                         // Service business logic classes (EntryService)
|    |- App.cpp                          // main() is here
|    |- SwaggerComponent.hpp             // Configuration for swagger-ui
|    |- AppComponent.hpp                 // Service configuration
|    |- DbComponent.hpp                  // Database configuration
|    
|- test/                                 // Folder containing tests
```

---

## Build and Run Using CMake

**Requires** 

- SQlite installed.
- `oatpp`, `oatpp-swagger` and `oatpp-sqlite` modules installed.

```
$ cd build
$ cmake ..
$ make 
$ ./phonebook-test  # - run tests.
$ ./phonebook-exe   # - run application.
```

### After run

Go to [http://localhost:8000/swagger/ui](http://localhost:8000/swagger/ui) to try endpoints.