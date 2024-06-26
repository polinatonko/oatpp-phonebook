#ifndef CREATE_ENTRY_COROUTINE_HPP
#define CREATE_ENTRY_COROUTINE_HPP

#include <atomic>

#include "oatpp/async/Coroutine.hpp"
#include "oatpp/macro/component.hpp"
#include "oatpp/web/protocol/http/Http.hpp"

#include "app/TestClient.hpp"
#include "../src/dto/DTOs.hpp"

namespace {

/**
 * Coroutine for testing Create Entry method.
 */
class CreateEntryCoroutine : public oatpp::async::Coroutine<CreateEntryCoroutine> {
    typedef oatpp::web::protocol::http::incoming::Response IncomingResponse;
    OATPP_COMPONENT(std::shared_ptr<TestClient>, client);
    OATPP_COMPONENT(std::shared_ptr<oatpp::web::mime::ContentMappers>, apiContentMappers);

public:
    static std::atomic<v_int32> SUCCESS_COUNTER;

    Action act() override {
        auto dto = EntryRequestDto::createShared();
        dto->name = "Mihail";
        dto->phone = "+375331234568";
        dto->address = "Lugovaya 1, Brest";
        
        return client->createEntryAsync(dto).callbackTo(&CreateEntryCoroutine::onResponse);
    }

    Action onResponse(const std::shared_ptr<IncomingResponse>& response) {
        OATPP_ASSERT(response->getStatusCode() == 201 && "CreateEntryCoroutine")
        
        return response->readBodyToDtoAsync<oatpp::Object<EntryDto>>(apiContentMappers->getMapper("application/json"))
                        .callbackTo(&CreateEntryCoroutine::onBodyRead);
    }

    Action onBodyRead(const oatpp::Object<EntryDto>& dto) {
        OATPP_ASSERT(dto)
        OATPP_ASSERT(dto->id == 4)
        OATPP_ASSERT(dto->name == "Mihail")
        OATPP_ASSERT(dto->phone == "+375331234568")
        OATPP_ASSERT(dto->address == "Lugovaya 1, Brest")
        ++SUCCESS_COUNTER;

        return finish();
    }

    Action handleError(Error* error) override {
        if(error->is<oatpp::AsyncIOError>()) {
            auto e = static_cast<oatpp::AsyncIOError*>(error);
            OATPP_LOGe("[AsyncTest::CreateEntryCoroutine::handleError()]", "AsyncIOError. {}, {}", e->what(), e->getCode())
        } else {
            OATPP_LOGe("[AsyncTest::CreateEntryCoroutine::handleError()]", "Error. {}", error->what())
        }
        OATPP_ASSERT(!"Error")

        return error;
    }
};

std::atomic<v_int32> CreateEntryCoroutine::SUCCESS_COUNTER(0);

}

#endif /* CREATE_ENTRY_COROUTINE_HPP */