#include "ErrorHandler.hpp"
#include "oatpp/base/Log.hpp"

const char* ErrorHandler::getErrorMessage(const std::exception &ex, Status& status, Headers& headers)
{
    OATPP_LOGi("MyApp:EntryService", "{}", ex.what());
    try {
        std::rethrow_if_nested(ex);
    }
    catch (const std::exception& nested) {
        return getErrorMessage(nested, status, headers);
    }

    status = Status::CODE_500;
    if (auto httpError = dynamic_cast<const oatpp::web::protocol::http::HttpError*>(std::addressof(ex))) {
        for(auto& h : httpError->getHeaders().getAll_Unsafe()) {
            headers.putIfNotExists(h.first.toString(), h.second.toString());
        }
        status = httpError->getInfo().status;
    }
    else if (auto vError = dynamic_cast<const ValidationException*>(std::addressof(ex))) {
        status = Status::CODE_400;
    }

    return ex.what();
}

std::shared_ptr<ErrorHandler::OutgoingResponse> ErrorHandler::handleError(const Status &status, const oatpp::String &message, const Headers &headers)
{
    auto statusDto = StatusDto::createShared();
    statusDto->code = status.code;
    statusDto->status = status.description;
    statusDto->message = message;

    auto response = ResponseFactory::createResponse(status, statusDto, m_objectMapper);
    for (auto h: headers.getAll()) {
        response->putHeader(h.first.toString(), h.second.toString());
    }

    return response;
}

std::shared_ptr<ErrorHandler::OutgoingResponse> ErrorHandler::handleError(const std::exception_ptr &exceptionPtr)
{
    auto statusDto = StatusDto::createShared();

    try {
        std::rethrow_exception(exceptionPtr);
    }
    catch (std::exception& ex) {
        Status status;
        Headers headers;
        auto message = getErrorMessage(ex, status, headers);
        
        return handleError(status, message, headers);
    }
}
