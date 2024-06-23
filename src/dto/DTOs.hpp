#ifndef DTOS_HPP
#define DTOS_HPP

#include "oatpp/macro/codegen.hpp"
#include "oatpp/macro/component.hpp"
#include "oatpp/Types.hpp"

#include OATPP_CODEGEN_BEGIN(DTO)

/**
 *  Data Transfer Object. Object containing fields only.
 *  Used in API for serialization/deserialization and validation
 */

class EntryDto : public oatpp::DTO {

  DTO_INIT(EntryDto, DTO)

  DTO_FIELD(UInt32, id);
  DTO_FIELD(String, name);
  DTO_FIELD_INFO(phone) {
    info->pattern = "+375XXXXXXXXX, where X - digit from 0 to 9";
  }
  DTO_FIELD(String, phone);
  DTO_FIELD(String, address);
  
};

template<typename T>
class PageDto : public oatpp::DTO {
  
  DTO_INIT(PageDto, DTO)
  
  DTO_FIELD(UInt32, pageNumber);
  DTO_FIELD(UInt32, limit);
  DTO_FIELD(Vector<T>, items);
  
};

class StatusDto : public oatpp::DTO {

  DTO_INIT(StatusDto, DTO)

  DTO_FIELD(Int32, code);
  DTO_FIELD(String, status);
  DTO_FIELD(String, message);

};

#include OATPP_CODEGEN_END(DTO)

#endif /* DTOS_HPP */
