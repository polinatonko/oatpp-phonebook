#ifndef DTOS_HPP
#define DTOS_HPP

#include "oatpp/macro/codegen.hpp"
#include "oatpp/macro/component.hpp"
#include "oatpp/Types.hpp"

#include OATPP_CODEGEN_BEGIN(DTO)

/**
 * Entry request dto.
 */
class EntryRequestDto : public oatpp::DTO {
  DTO_INIT(EntryRequestDto, DTO)

  DTO_FIELD(String, name);
  DTO_FIELD_INFO(phone) {
    info->pattern = "+375XXXXXXXXX, where X - digit from 0 to 9";
  }
  DTO_FIELD(String, phone);
  DTO_FIELD(String, address);
};

/**
 * Entry dto that stores in daatabase.
 */
class EntryDto : public oatpp::DTO {
  DTO_INIT(EntryDto, DTO)

  DTO_FIELD(Int32, id);
  DTO_FIELD(String, name);
  DTO_FIELD_INFO(phone) {
    info->pattern = "+375XXXXXXXXX, where X - digit from 0 to 9";
  }
  DTO_FIELD(String, phone);
  DTO_FIELD(String, address);
};


/**
 * Dto to store page of entries.
 */
template<typename T>
class PageDto : public oatpp::DTO {
  
  DTO_INIT(PageDto, DTO)

  DTO_FIELD_INFO(pageNumber) { info->description = "Number of requested page"; }
  DTO_FIELD_INFO(limit) { info->description = "Maximum number of entries per page"; }
  DTO_FIELD_INFO(count) { info->description = "Number of actually returned items"; }
  DTO_FIELD_INFO(items) { info->description = "Collection of requested Entry items"; }
  
  DTO_FIELD(Int32, pageNumber);
  DTO_FIELD(Int32, limit);
  DTO_FIELD(Int32, count);
  DTO_FIELD(Vector<T>, items);
  
};

/**
 * API response status dto.
 */
class StatusDto : public oatpp::DTO {

  DTO_INIT(StatusDto, DTO)

  DTO_FIELD_INFO(code) { info->description = "Status code"; }
  DTO_FIELD_INFO(status) { info->description = "Short status text"; }
  DTO_FIELD_INFO(message) { info->description = "Verbose message"; }

  DTO_FIELD(Int32, code);
  DTO_FIELD(String, status);
  DTO_FIELD(String, message);

};

#include OATPP_CODEGEN_END(DTO)

#endif /* DTOS_HPP */
