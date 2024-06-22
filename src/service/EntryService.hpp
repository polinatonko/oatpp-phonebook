#ifndef EntryService_hpp
#define EntryService_hpp

#include "oatpp/web/protocol/http/Http.hpp"
#include "oatpp/macro/component.hpp"

#include "dto/DTOs.hpp"
#include "dto/Validators.hpp"

class EntryService {
    typedef oatpp::web::protocol::http::Status Status;
    std::unordered_set<oatpp::Object<EntryDto>> entries;
    Validators::EntryValidator entryValidator;

public:
    EntryService();
    void init();
    void validate(const oatpp::Object<EntryDto>& dto);
    oatpp::Object<EntryDto> createEntry(const oatpp::Object<EntryDto>& dto);
    oatpp::Object<EntryDto> updateEntry(const oatpp::Object<EntryDto>& dto);
    oatpp::Object<EntryDto> getEntryById(const oatpp::Int32& id);
    void deleteEntryById(const oatpp::Int32& id);
    oatpp::Object<PageDto<oatpp::Object<EntryDto>>> getEntries(const oatpp::Int32& offset, const oatpp::Int32& limit);
};

#endif