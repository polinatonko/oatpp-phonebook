
#ifndef TEST_CLIENT_HPP
#define TEST_CLIENT_HPP

#include "oatpp/web/client/ApiClient.hpp"
#include "oatpp/macro/codegen.hpp"

#include "../../src/dto/DTOs.hpp"

namespace {

#include OATPP_CODEGEN_BEGIN(ApiClient) // Begin code generation

  /**
   * Test API client.
   */
  class TestClient : public oatpp::web::client::ApiClient {

    API_CLIENT_INIT(TestClient)

    API_CALL_ASYNC("GET", "/entries", getEntriesAsync, QUERY(Int32, page), QUERY(Int32, limit))
    API_CALL_ASYNC("GET", "/entry/{id}", getEntryByIdAsync, PATH(Int32, id))
    API_CALL_ASYNC("PUT", "/entry/{id}", 
                    updateEntryAsync, PATH(Int32, id), BODY_DTO(Object<EntryRequestDto>, dto))
    API_CALL_ASYNC("DELETE", "/entry/{id}", deleteEntryAsync, PATH(Int32, id))
    API_CALL_ASYNC("POST", "/entries", createEntryAsync, BODY_DTO(Object<EntryRequestDto>, dto))

  };

#include OATPP_CODEGEN_END(ApiClient) // End code generation
}

#endif /* TEST_CLIENT_HPP */