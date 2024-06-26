#ifndef GET_ENTRIES_COROUTINE_HPP
#define GET_ENTRIES_COROUTINE_HPP

#include <atomic>

#include "oatpp/async/Coroutine.hpp"
#include "oatpp/macro/component.hpp"
#include "oatpp/web/protocol/http/Http.hpp"

#include "app/TestClient.hpp"
#include "../src/dto/DTOs.hpp"

namespace {

/**
 * Coroutine for testing Get Entries method.
 */
class GetEntriesCoroutine : public oatpp::async::Coroutine<GetEntriesCoroutine> {
    typedef oatpp::web::protocol::http::incoming::Response IncomingResponse;
    OATPP_COMPONENT(std::shared_ptr<TestClient>, client);
    OATPP_COMPONENT(std::shared_ptr<oatpp::web::mime::ContentMappers>, apiContentMappers);

public:
    static std::atomic<v_int32> SUCCESS_COUNTER;

    Action act() override {
        return client->getEntriesAsync(1, 4).callbackTo(&GetEntriesCoroutine::onResponse);
    }

    Action onResponse(const std::shared_ptr<IncomingResponse>& response) {
        OATPP_ASSERT(response->getStatusCode() == 200 && "GetEntriesCoroutine")
        
        return response->readBodyToDtoAsync<oatpp::Object<PageDto<oatpp::Object<EntryDto>>>>(apiContentMappers->getMapper("application/json"))
                        .callbackTo(&GetEntriesCoroutine::onBodyRead);
    }

    Action onBodyRead(const oatpp::Object<PageDto<oatpp::Object<EntryDto>>>& dto) {
        OATPP_ASSERT(dto)
        OATPP_ASSERT(dto->limit == 4)
        OATPP_ASSERT(dto->pageNumber == 1)
        OATPP_ASSERT(dto->count == 3)

        OATPP_ASSERT(dto->items[0]->id == 1)
        OATPP_ASSERT(dto->items[0]->name == "Ivan")
        OATPP_ASSERT(dto->items[0]->phone == "+375295556688")
        OATPP_ASSERT(dto->items[0]->address == "Minsk")

        OATPP_ASSERT(dto->items[1]->id == 2)
        OATPP_ASSERT(dto->items[1]->name == "Olga")
        OATPP_ASSERT(dto->items[1]->phone == "+375295556689")
        OATPP_ASSERT(dto->items[1]->address == "Kopyl")

        OATPP_ASSERT(dto->items[2]->id == 3)
        OATPP_ASSERT(dto->items[2]->name == "Boris")
        OATPP_ASSERT(dto->items[2]->phone == "+375295556680")
        OATPP_ASSERT(dto->items[2]->address == "Grodno")

        ++SUCCESS_COUNTER;

        return finish();
    }

    Action handleError(Error* error) override {
        if(error->is<oatpp::AsyncIOError>()) {
            auto e = static_cast<oatpp::AsyncIOError*>(error);
            OATPP_LOGe("[AsyncTest::GetEntriesCoroutine::handleError()]", "AsyncIOError. {}, {}", e->what(), e->getCode())
        } else {
            OATPP_LOGe("[AsyncTest::GetEntriesCoroutine::handleError()]", "Error. {}", error->what())
        }
        OATPP_ASSERT(!"Error")
        
        return error;
    }
};

std::atomic<v_int32> GetEntriesCoroutine::SUCCESS_COUNTER(0);

}

#endif /* GET_ENTRIES_COROUTINE_HPP */