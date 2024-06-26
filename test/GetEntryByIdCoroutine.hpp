#ifndef GET_ENTRY_BY_ID_COROUTINE_HPP
#define GET_ENTRY_BY_ID_COROUTINE_HPP

#include <atomic>

#include "oatpp/async/Coroutine.hpp"
#include "oatpp/macro/component.hpp"
#include "oatpp/web/protocol/http/Http.hpp"

#include "app/TestClient.hpp"
#include "../src/dto/DTOs.hpp"

namespace {

/**
 * Coroutine for testing Get Entry method.
 */
class GetEntryByIdCoroutine : public oatpp::async::Coroutine<GetEntryByIdCoroutine> {
    typedef oatpp::web::protocol::http::incoming::Response IncomingResponse;
    OATPP_COMPONENT(std::shared_ptr<TestClient>, client);
    OATPP_COMPONENT(std::shared_ptr<oatpp::web::mime::ContentMappers>, apiContentMappers);

public:
    static std::atomic<v_int32> SUCCESS_COUNTER;

    Action act() override {
        return client->getEntryByIdAsync(2).callbackTo(&GetEntryByIdCoroutine::onResponse);
    }

    Action onResponse(const std::shared_ptr<IncomingResponse>& response) {
        OATPP_ASSERT(response->getStatusCode() == 200 && "GetEntryByIdCoroutine")
        
        return response->readBodyToDtoAsync<oatpp::Object<EntryDto>>(apiContentMappers->getMapper("application/json"))
                        .callbackTo(&GetEntryByIdCoroutine::onBodyRead);
    }

    Action onBodyRead(const oatpp::Object<EntryDto>& dto) {
        OATPP_ASSERT(dto)
        OATPP_ASSERT(dto->id == 2)
        OATPP_ASSERT(dto->name == "Olga")
        OATPP_ASSERT(dto->phone == "+375295556689")
        OATPP_ASSERT(dto->address == "Kopyl")

        ++SUCCESS_COUNTER;

        return finish();
    }

    Action handleError(Error* error) override {
        if(error->is<oatpp::AsyncIOError>()) {
            auto e = static_cast<oatpp::AsyncIOError*>(error);
            OATPP_LOGe("[AsyncTest::GetEntryByIdCoroutine::handleError()]", "AsyncIOError. {}, {}", e->what(), e->getCode())
        } else {
            OATPP_LOGe("[AsyncTest::GetEntryByIdCoroutine::handleError()]", "Error. {}", error->what())
        }
        OATPP_ASSERT(!"Error")
        
        return error;
    }
};

std::atomic<v_int32> GetEntryByIdCoroutine::SUCCESS_COUNTER(0);

}

#endif /* GET_ENTRY_BY_ID_COROUTINE_HPP */