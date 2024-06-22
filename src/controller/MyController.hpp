#ifndef MyController_hpp
#define MyController_hpp

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

  ENDPOINT("GET", "/entry/{id}", getEntryById,
           PATH(Int32, id)) {
    return createDtoResponse(Status::CODE_200, entryService.getEntryById(id));
  }

  ENDPOINT("POST", "/entries", createEntry,
           BODY_DTO(Object<EntryDto>, dto)) {
    entryService.validate(dto);
    auto response = createDtoResponse(Status::CODE_201, entryService.createEntry(dto));
    response->putHeader("Content-Location", "/entry/" + oatpp::utils::Conversion::int32ToStr(dto->id));

    return response;
  }

  ENDPOINT("PUT", "/entry", updateEntry,
           BODY_DTO(Object<EntryDto>, dto)) {
    entryService.validate(dto);
    auto response = createDtoResponse(Status::CODE_201, entryService.createEntry(dto));
    response->putHeader("Content-Location", "/entry/" + oatpp::utils::Conversion::int32ToStr(dto->id));

    return createDtoResponse(Status::CODE_200, entryService.updateEntry(dto));
  }

  ENDPOINT("DELETE", "/entry/{id}", deleteEntry,
           PATH(Int32, id)) {
    entryService.deleteEntryById(id);

    return createResponse(Status::CODE_204);
  }
  
  ENDPOINT("GET", "/entries/{offset}/{limit}", getAllEntries,
           PATH(Int32, offset),
           PATH(Int32, limit)) {
    return createDtoResponse(Status::CODE_200, entryService.getEntries(offset, limit));
  }
  
};

#include OATPP_CODEGEN_END(ApiController) //<-- End Codegen

#endif /* MyController_hpp */
