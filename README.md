# oatpp-phonebook

Phonebook API built with Oat++, using oatpp ORM with SQLite and documented with Swagger and OpenApi 3.0.0.

<img src="https://github.com/user-attachments/assets/7330a2b8-2627-478f-8e33-11f4c3cabd7c" width="405"><img src="https://github.com/user-attachments/assets/1b4fb0a8-3bc3-4b3d-a685-37811a86f31f" width="405">

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
|- sql/                                  // Database migrations
|- utility/                              // Folder contains script to install oatpp modules
```

---

## Build and Run Using CMake

**Requires** 

- SQlite installed.
- `oatpp`, `oatpp-swagger` and `oatpp-sqlite` modules installed.
**Note:** You may run `utility/install-oatpp-modules.sh` script to install required oatpp modules.

```
$ mkdir build && cd build
$ cmake ..
$ make 
$ ./phonebook-test  # - run tests.
$ ./phonebook-exe   # - run application.
```

### After run

Go to [http://localhost:8000/swagger/ui](http://localhost:8000/swagger/ui) to try endpoints.

---

### Endpoints 

#### HTML

|HTTP Method|URL|Description|
|---|---|---|
|`GET`|http://localhost:8000/swagger/ui | Swagger UI page |

#### Entry Service

|HTTP Method|URL|Description|
|---|---|---|
|`POST`|http://localhost:8000/entries | Create new Entry |
|`PUT`|http://localhost:8000/entry/{id} | Update Entry by ID |
|`GET`|http://localhost:8000/entry/{id} | Get Entry by ID |
|`DELETE`|http://localhost:8000/entry/{id} | Delete Entry by ID |
|`GET`|http://localhost:8000/entries?page={page}&limit={limit} | Get All Entries with Paging |
