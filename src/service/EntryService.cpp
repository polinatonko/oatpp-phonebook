#include "EntryService.hpp"


void EntryService::validate(const oatpp::Object<EntryDto> &dto)
{
    entryValidator.validate(dto);
}

oatpp::Object<EntryDto> EntryService::createEntry(const oatpp::Object<EntryDto>& dto) {
    auto dbResult = entryDb->createEntry(dto);
    OATPP_ASSERT_HTTP(dbResult->isSuccess(), Status::CODE_500, dbResult->getErrorMessage());

    auto entryId = oatpp::sqlite::Utils::getLastInsertRowId(dbResult->getConnection());
    OATPP_LOGi("MyApp:EntryService", "Create entry id={}", entryId);

    return getEntryById(entryId);
}

oatpp::Object<EntryDto> EntryService::updateEntry(const oatpp::Object<EntryDto>& dto) {
    auto dbResult = entryDb->updateEntry(dto);
    OATPP_ASSERT_HTTP(dbResult->isSuccess(), Status::CODE_500, dbResult->getErrorMessage());
    OATPP_LOGi("MyApp:EntryService", "Update entry id={}", dto->id);

    return getEntryById(dto->id);
}

oatpp::Object<EntryDto> EntryService::getEntryById(const oatpp::Int32& id) {
    auto dbResult = entryDb->getEntryById(id);
    OATPP_ASSERT_HTTP(dbResult->isSuccess(), Status::CODE_500, dbResult->getErrorMessage());
    OATPP_ASSERT_HTTP(dbResult->hasMoreToFetch(), Status::CODE_404, "Entry not found");

    auto result = dbResult->fetch<oatpp::Vector<oatpp::Object<EntryDto>>>();
    OATPP_ASSERT_HTTP(result->size() == 1, Status::CODE_500, "There are multiple entries with same id");
    OATPP_LOGi("MyApp:EntryService", "Get entry id={}", id);

    return result[0];
}

void EntryService::deleteEntryById(const oatpp::Int32& id) {
    OATPP_LOGi("My App", "Inside delete {}", id);
    auto dbResult = entryDb->deleteEntry(id);
    OATPP_ASSERT_HTTP(dbResult->isSuccess(), Status::CODE_500, dbResult->getErrorMessage());
    OATPP_LOGi("MyApp:EntryService", "Delete entry id={}", id);
}

oatpp::Object<PageDto<oatpp::Object<EntryDto>>> EntryService::getEntries(const oatpp::Int32& pageNumber, const oatpp::Int32& limit) {
    oatpp::Int32 offset = (pageNumber - 1) * limit;
    auto dbResult = entryDb->getEntries(limit, offset);
    OATPP_ASSERT_HTTP(dbResult->isSuccess(), Status::CODE_500, dbResult->getErrorMessage());

    auto result = dbResult->fetch<oatpp::Vector<oatpp::Object<EntryDto>>>();
    auto page = PageDto<oatpp::Object<EntryDto>>::createShared();
    page->limit = limit;
    page->pageNumber = pageNumber;
    page->items = result;

    return page;
}