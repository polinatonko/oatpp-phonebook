#ifndef MY_CONTROLLER_HPP
#define MY_CONTROLLER_HPP

#include "oatpp/web/server/api/ApiController.hpp"
#include "oatpp/utils/Conversion.hpp"

#include "dto/DTOs.hpp"
#include "service/EntryService.hpp"

#include OATPP_CODEGEN_BEGIN(ApiController) //<-- Begin Codegen


/**
 * Phone Entry Api Controller.
 */
class MyController : public oatpp::web::server::api::ApiController {
  EntryService entryService;
  

public:
  /**
   * Constructor with object mapper.
   * @param apiContentMappers - mappers used to serialize/deserialize DTOs.
   */
  MyController(OATPP_COMPONENT(std::shared_ptr<oatpp::web::mime::ContentMappers>, apiContentMappers))
    : oatpp::web::server::api::ApiController(apiContentMappers)
  {}
public:

  ENDPOINT_INFO(getEntryById) {
    info->summary = "Get one Entry by id";

    info->addResponse<Object<EntryDto>>(Status::CODE_200, "application/json");
    info->addResponse(Status::CODE_404, "application/json");
    info->addResponse(Status::CODE_500, "application/json");

    info->pathParams["id"].description = "Phone entry identifier";
  }
  ENDPOINT("GET", "/entry/{id}", getEntryById,
           PATH(UInt32, id)) {
    return createDtoResponse(Status::CODE_200, entryService.getEntryById(id));
  }

  ENDPOINT_INFO(createEntry) {
    info->summary = "Create Entry";

    info->addConsumes<Object<EntryDto>>("application/json");

    info->addResponse<Object<EntryDto>>(Status::CODE_201, "application/json");
    info->addResponse(Status::CODE_500, "application/json");
  }
  ENDPOINT("POST", "/entries", createEntry,
           BODY_DTO(Object<EntryDto>, dto)) {
    entryService.validate(dto);
    oatpp::Object<EntryDto> created = entryService.createEntry(dto);
    auto response = createDtoResponse(Status::CODE_201, created);
    response->putHeader("Content-Location", "/entry/" + oatpp::utils::Conversion::int32ToStr(created->id));

    return response;
  }

  ENDPOINT_INFO(updateEntry) {
    info->summary = "Update Entry by id";

    info->addConsumes<Object<EntryDto>>("application/json");

    info->addResponse<Object<EntryDto>>(Status::CODE_200, "application/json");
    info->addResponse(Status::CODE_500, "application/json");

    info->pathParams["id"].description = "Phone entry identifier";
  }
  ENDPOINT("PUT", "/entry", updateEntry,
           BODY_DTO(Object<EntryDto>, dto)) {
    entryService.validate(dto);
    auto response = createDtoResponse(Status::CODE_201, entryService.createEntry(dto));
    response->putHeader("Content-Location", "/entry/" + oatpp::utils::Conversion::int32ToStr(dto->id));

    return createDtoResponse(Status::CODE_200, entryService.updateEntry(dto));
  }

  ENDPOINT_INFO(deleteEntry) {
    info->summary = "Delete Entry by id";

    info->addResponse(Status::CODE_204);
    info->addResponse(Status::CODE_500, "application/json");

    info->pathParams["id"].description = "Phone entry identifier";
  }
  ENDPOINT("DELETE", "/entry/{id}", deleteEntry,
           PATH(UInt32, id)) {
    entryService.deleteEntryById(id);

    return createResponse(Status::CODE_204);
  }
  
  ENDPOINT_INFO(getAllEntries) {
    info->summary = "Get multiple entries";

    info->addResponse<Object<PageDto<Object<EntryDto>>>>(Status::CODE_200, "application/json");
    info->addResponse(Status::CODE_500, "application/json");

    info->pathParams["page"].description = "Page number (positive integer)";
    info->pathParams["limit"].description = "Entries number per page";
  }
  ENDPOINT("GET", "/entries/{page}/{limit}", getAllEntries,
           PATH(UInt32, page),
           PATH(UInt32, limit)) {
    return createDtoResponse(Status::CODE_200, entryService.getEntries(page, limit));
  }
  
};

#include OATPP_CODEGEN_END(ApiController) //<-- End Codegen

#endif /* MY_CONTROLLER_HPP */
