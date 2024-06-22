#include "EntryService.hpp"

EntryService::EntryService() {
    init();
}

void EntryService::init() {
    auto obj = oatpp::Object<EntryDto>::createShared();
    obj->id = 1;
    obj->phone = "+375296667788";
    obj->name = "Ivan";
    obj->address = "Minsk";
    entries.insert(obj);
}

void EntryService::validate(const oatpp::Object<EntryDto> &dto)
{
    entryValidator.validate(dto);

    OATPP_ASSERT_HTTP(getEntryById(dto->id) == nullptr, oatpp::web::protocol::http::Status::CODE_400, "Entry with such id already exist.");
}

oatpp::Object<EntryDto> EntryService::createEntry(const oatpp::Object<EntryDto>& dto) {
    entries.insert(dto);

    return dto;
}

auto findById = [](const oatpp::Object<EntryDto>& dto, oatpp::Int32 id) {
    return dto->id == id;
};

oatpp::Object<EntryDto> EntryService::updateEntry(const oatpp::Object<EntryDto>& dto) {
    auto entry = getEntryById(dto->id);
    entry->phone = dto->phone;
    entry->name = dto->name;
    entry->address = dto->address;

    return entry;
}

oatpp::Object<EntryDto> EntryService::getEntryById(const oatpp::Int32& id) {
    v_int32 v_id = id.getValue(1);
    auto it = std::find_if(entries.begin(), entries.end(), [v_id](const oatpp::Object<EntryDto> obj) {
        return obj->id == v_id;
    });

    return (it != entries.end()) ? *it : nullptr;
}

void EntryService::deleteEntryById(const oatpp::Int32& id) {
    v_int32 v_id = id.getValue(1);
    entries.erase(getEntryById(id));
}

oatpp::Object<PageDto<oatpp::Object<EntryDto>>> EntryService::getEntries(const oatpp::Int32& offset, const oatpp::Int32& limit) {
    auto it = entries.begin();
    for (int i = 0; i < offset && it != entries.end(); ++i)
        it++;

    auto page = PageDto<oatpp::Object<EntryDto>>::createShared();
    page->offset = offset;
    page->limit = limit;
    page->items = oatpp::Vector<oatpp::Object<EntryDto>>::createShared();
    
    for (int i = 0; i < limit && it != entries.end(); ++i) {
        page->items->push_back(*it);
        it++;
    }

    return page;
}