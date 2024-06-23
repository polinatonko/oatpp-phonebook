#ifndef EntryService_hpp
#define EntryService_hpp

#include "oatpp/web/protocol/http/Http.hpp"
#include "oatpp/macro/component.hpp"
#include "oatpp/base/Log.hpp"

#include "dto/DTOs.hpp"
#include "dto/Validators.hpp"
#include "db/EntryDb.hpp"

class EntryService {
    typedef oatpp::web::protocol::http::Status Status;
    OATPP_COMPONENT(std::shared_ptr<EntryDb>, entryDb);
    Validators::EntryValidator entryValidator;

public:
    void validate(const oatpp::Object<EntryDto>& dto);
    oatpp::Object<EntryDto> createEntry(const oatpp::Object<EntryDto>& dto);
    oatpp::Object<EntryDto> updateEntry(const oatpp::Object<EntryDto>& dto);
    oatpp::Object<EntryDto> getEntryById(const oatpp::UInt32& id);
    void deleteEntryById(const oatpp::UInt32& id);
    oatpp::Object<PageDto<oatpp::Object<EntryDto>>> getEntries(const oatpp::UInt32& page, const oatpp::UInt32& limit);
};

#endif