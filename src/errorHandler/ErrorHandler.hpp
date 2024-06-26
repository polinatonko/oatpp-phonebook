#ifndef ERROR_HANDLER_HPP
#define ERROR_HANDLER_HPP

#include "oatpp/web/protocol/http/outgoing/ResponseFactory.hpp"
#include "oatpp/web/server/handler/ErrorHandler.hpp"
#include "oatpp/web/protocol/http/Http.hpp"
#include "oatpp/base/Log.hpp"

#include "Exception.hpp"
#include "../dto/DTOs.hpp"

/**
 * Custom error handler.
 */
class ErrorHandler : public oatpp::web::server::handler::ErrorHandler {
    typedef oatpp::web::protocol::http::outgoing::Response OutgoingResponse;
    typedef oatpp::web::protocol::http::Status Status;
    typedef oatpp::web::protocol::http::outgoing::ResponseFactory ResponseFactory;

    std::shared_ptr<oatpp::data::mapping::ObjectMapper> objectMapper;

    const char* getErrorMessage(const std::exception& ex, Status& status, Headers& headers);

public:
    /**
     * Constructor with object mapper.
     * @param objectMapper - mapper used to serialize/deserialize DTOs.
     */
    ErrorHandler(const std::shared_ptr<oatpp::data::mapping::ObjectMapper>& objectMapper)
      : objectMapper(objectMapper)
    {}

    std::shared_ptr<OutgoingResponse> handleError(const Status &status, const oatpp::String &message, const Headers &headers);

    std::shared_ptr<OutgoingResponse> handleError(const std::exception_ptr& exceptionPtr) override;
};

#endif /* ERROR_HANDLER_HPP */