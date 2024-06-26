#ifndef MY_CONTROLLER_HPP
#define MY_CONTROLLER_HPP

#include "oatpp/web/server/api/ApiController.hpp"
#include "oatpp/utils/Conversion.hpp"

#include "service/EntryService.hpp"

#include OATPP_CODEGEN_BEGIN(ApiController) //<-- Begin Codegen

/**
 * Phone Entry API Controller.
 */
class MyController : public oatpp::web::server::api::ApiController {
  EntryService entryService;
  typedef MyController __ControllerType;

public:
  /**
   * Constructor with object mapper.
   * @param apiContentMappers - mappers used to serialize/deserialize DTOs.
   */
  MyController(OATPP_COMPONENT(std::shared_ptr<oatpp::web::mime::ContentMappers>, apiContentMappers))
    : oatpp::web::server::api::ApiController(apiContentMappers)
  {}

  ENDPOINT_INFO(getEntryByIdAsync) {
    info->summary = "Get one Entry by given identifier";

    info->pathParams.add<oatpp::Int32>("id").required = true;
    info->pathParams["id"].description = "Identifier of the Entry";
    info->pathParams["id"].addExample("1", Int32(12));

    info->addResponse<Object<EntryDto>>(Status::CODE_200, "application/json");
    info->addResponse<Object<StatusDto>>(Status::CODE_404, "application/json");
    info->addResponse<Object<StatusDto>>(Status::CODE_500, "application/json");
  }
  ENDPOINT_ASYNC("GET", "/entry/{id}", getEntryByIdAsync) {

    ENDPOINT_ASYNC_INIT(getEntryByIdAsync);

    Action act() override {
      oatpp::Int32 id = oatpp::utils::Conversion::strToInt32(request->getPathVariable("id")->c_str());

      auto entry = controller->entryService.getEntryById(id);
      OATPP_ASSERT_HTTP(entry, Status::CODE_404, "Entry with such identifier not found");

      return _return(controller->createDtoResponse(Status::CODE_200, entry));
    }
  };

  ENDPOINT_INFO(createEntryAsync) {
    info->summary = "Create new Entry";

    info->addConsumes<Object<EntryRequestDto>>("application/json");

    info->addResponse<Object<EntryDto>>(Status::CODE_201, "application/json");
    info->addResponse<Object<StatusDto>>(Status::CODE_400, "application/json");
    info->addResponse<Object<StatusDto>>(Status::CODE_500, "application/json");
  }
  ENDPOINT_ASYNC("POST", "/entries", createEntryAsync) {

    ENDPOINT_ASYNC_INIT(createEntryAsync);

    Action act() override {
      return request->readBodyToDtoAsync<Object<EntryRequestDto>>(controller->getContentMappers()->getDefaultMapper())
                      .callbackTo(&createEntryAsync::createEntryResponse);
    }

    Action createEntryResponse(const Object<EntryRequestDto>& dto) {
      controller->entryService.validate(dto);

      auto created = controller->entryService.createEntry(dto);
      auto response = controller->createDtoResponse(Status::CODE_201, created);
      response->putHeader("Content-Location", "/entry/" + oatpp::utils::Conversion::int32ToStr(created->id));

      return _return(response);
    }
  };

  ENDPOINT_INFO(updateEntryAsync) {
    info->summary = "Update existing Entry by given identifier";

    info->pathParams.add<oatpp::Int32>("id").required = true;
    info->pathParams["id"].description = "Identifier of the Entry";
    info->pathParams["id"].addExample("1", Int32(12));

    info->addConsumes<Object<EntryRequestDto>>("application/json");

    info->addResponse<Object<EntryDto>>(Status::CODE_200, "application/json");
    info->addResponse<Object<StatusDto>>(Status::CODE_400, "application/json").description = "Request body validation error";
    info->addResponse<Object<StatusDto>>(Status::CODE_404, "application/json");
    info->addResponse<Object<StatusDto>>(Status::CODE_500, "application/json");
  }
  ENDPOINT_ASYNC("PUT", "/entry/{id}", updateEntryAsync) {

    ENDPOINT_ASYNC_INIT(updateEntryAsync);

    Int32 id;

    Action act() override {
      id = oatpp::utils::Conversion::strToInt32(request->getPathVariable("id")->c_str());

      return request->readBodyToDtoAsync<Object<EntryRequestDto>>(controller->getContentMappers()->getDefaultMapper())
                      .callbackTo(&updateEntryAsync::updateEntryResponse);
    } 

    Action updateEntryResponse(const Object<EntryRequestDto>& dto) {
      controller->entryService.validate(dto);
      auto entry = controller->entryService.updateEntry(id, dto);
      OATPP_ASSERT_HTTP(entry, Status::CODE_404, "Entry with such identifier not found");

      return _return(controller->createDtoResponse(Status::CODE_200, entry));
    }
  };

  ENDPOINT_INFO(deleteEntryAsync) {
    info->summary = "Delete Entry by given identifier";

    info->pathParams.add<oatpp::Int32>("id").required = true;
    info->pathParams["id"].description = "Identifier of the Entry";
    info->pathParams["id"].addExample("1", Int32(12));

    info->addResponse(Status::CODE_204).description = "Request was successfully processed";
    info->addResponse<Object<StatusDto>>(Status::CODE_500, "application/json");
  }
  ENDPOINT_ASYNC("DELETE", "/entry/{id}", deleteEntryAsync) {

    ENDPOINT_ASYNC_INIT(deleteEntryAsync);

    Action act() override {
      Int32 id = oatpp::utils::Conversion::strToInt32(request->getPathVariable("id")->c_str());

      controller->entryService.deleteEntryById(id);
      
      return _return(controller->createResponse(Status::CODE_204));
    }
  };
  
  ENDPOINT_INFO(getEntriesAsync) {
    info->summary = "Get multiple entries";

    info->addResponse<Object<PageDto<Object<EntryDto>>>>(Status::CODE_200, "application/json");
    info->addResponse<Object<StatusDto>>(Status::CODE_400, "application/json");
    info->addResponse<Object<StatusDto>>(Status::CODE_500, "application/json");

    info->queryParams.add<oatpp::Int32>("page").required = false;
    info->queryParams["page"].description = "Page number (positive integer, default 1)";
    info->queryParams["page"].addExample("1", oatpp::Int32(5));

    info->queryParams.add<oatpp::Int32>("limit").required = false;
    info->queryParams["limit"].description = "Max value of entries number per page (default 10)";
    info->queryParams["limit"].addExample("1", oatpp::Int32(20));
  }
  ENDPOINT_ASYNC("GET", "/entries", getEntriesAsync) {

    ENDPOINT_ASYNC_INIT(getEntriesAsync);

     Action act() override {
      Int32 page = nullptr, limit = nullptr;
      if (request->getQueryParameter("page") != nullptr) {
        page = oatpp::utils::Conversion::strToInt32(request->getQueryParameter("page")->c_str());
        OATPP_ASSERT_HTTP(page > 0, Status::CODE_400, "Query parameter page number should be positive integer");
      }

      if (request->getQueryParameter("limit") != nullptr) {
        limit = oatpp::utils::Conversion::strToInt32(request->getQueryParameter("limit")->c_str());
        OATPP_ASSERT_HTTP(limit > 0, Status::CODE_400, "Query parameter limit should positive integer");
      }

      return _return(controller->createDtoResponse(Status::CODE_200, controller->entryService.getEntries(page, limit)));
    }
  };
  
};

#include OATPP_CODEGEN_END(ApiController) //<-- End Codegen

#endif /* MY_CONTROLLER_HPP */
