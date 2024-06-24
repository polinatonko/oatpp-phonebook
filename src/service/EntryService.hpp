#ifndef ENTRY_SERVICE_HPP
#define ENTRY_SERVICE_HPP

#include <unistd.h>
#include <regex>

#include "oatpp/web/protocol/http/Http.hpp"
#include "oatpp/macro/component.hpp"
#include "oatpp/base/Log.hpp"

#include "dto/DTOs.hpp"
#include "dto/Validators.hpp"
#include "db/EntryDb.hpp"
#include "errorHandler/Exception.hpp"

#define DEFAULT_LIMIT 10
#define DEFAULT_PAGE_NUMBER 1

class EntryService {
    typedef oatpp::web::protocol::http::Status Status;
    Validators::EntryValidator validator;    
    OATPP_COMPONENT(std::shared_ptr<EntryDb>, entryDb);

    void checkDbResult(const std::shared_ptr<oatpp::orm::QueryResult>& res);

public:

    void validate(const oatpp::Object<EntryRequestDto>& dto);
    oatpp::Object<EntryDto> createEntry(const oatpp::Object<EntryRequestDto>& dto);
    oatpp::Object<EntryDto> updateEntry(const oatpp::Int32& id, const oatpp::Object<EntryRequestDto>& dto);
    oatpp::Object<EntryDto> getEntryById(const oatpp::Int32& id);
    void deleteEntryById(const oatpp::Int32& id);
    oatpp::Object<PageDto<oatpp::Object<EntryDto>>> getEntries(oatpp::Int32& page, oatpp::Int32& limit);
};

#endif /* ENTRY_SERVICE_HPP */