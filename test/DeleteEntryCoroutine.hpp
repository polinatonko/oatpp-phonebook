#ifndef DELETE_ENTRY_COROUTINE_HPP
#define DELETE_ENTRY_COROUTINE_HPP

#include <atomic>

#include "oatpp/async/Coroutine.hpp"
#include "oatpp/macro/component.hpp"
#include "oatpp/web/protocol/http/Http.hpp"

#include "app/TestClient.hpp"
#include "../src/dto/DTOs.hpp"

namespace {

/**
 * Coroutine for testing Delete Entry method;
 */
class DeleteEntryCoroutine : public oatpp::async::Coroutine<DeleteEntryCoroutine> {
    typedef oatpp::web::protocol::http::incoming::Response IncomingResponse;
    OATPP_COMPONENT(std::shared_ptr<TestClient>, client);
    OATPP_COMPONENT(std::shared_ptr<oatpp::web::mime::ContentMappers>, apiContentMappers);

public:
    static std::atomic<v_int32> SUCCESS_COUNTER;

    Action act() override {
        return client->deleteEntryAsync(4).callbackTo(&DeleteEntryCoroutine::onResponse);
    }

    Action onResponse(const std::shared_ptr<IncomingResponse>& response) {
        OATPP_ASSERT(response->getStatusCode() == 204 && "DeleteEntryCoroutine")
        ++SUCCESS_COUNTER;

        return finish();
    }

    Action handleError(Error* error) override {
        if(error->is<oatpp::AsyncIOError>()) {
            auto e = static_cast<oatpp::AsyncIOError*>(error);
            OATPP_LOGe("[AsyncTest::DeleteEntryCoroutine::handleError()]", "AsyncIOError. {}, {}", e->what(), e->getCode())
        } else {
            OATPP_LOGe("[AsyncTest::DeleteEntryCoroutine::handleError()]", "Error. {}", error->what())
        }
        OATPP_ASSERT(!"Error")
        
        return error;
    }
};

std::atomic<v_int32> DeleteEntryCoroutine::SUCCESS_COUNTER(0);

}

#endif /* DELETE_ENTRY_COROUTINE_HPP */