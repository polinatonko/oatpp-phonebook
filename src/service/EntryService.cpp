#include "EntryService.hpp"

void EntryService::validate(const oatpp::Object<EntryRequestDto> &dto)
{
    validator.validate(dto);
}

oatpp::Object<EntryDto> EntryService::createEntry(const oatpp::Object<EntryRequestDto>& dto) {
    auto dbResult = entryDb->createEntry(dto);
    checkDbResult(dbResult);

    auto entryId = oatpp::sqlite::Utils::getLastInsertRowId(dbResult->getConnection());

    return getEntryById(entryId);
}

oatpp::Object<EntryDto> EntryService::updateEntry(const oatpp::Int32& id, const oatpp::Object<EntryRequestDto>& dto) {
    auto dbResult = entryDb->updateEntry(id, dto);
    checkDbResult(dbResult);

    return getEntryById(id);
}

oatpp::Object<EntryDto> EntryService::getEntryById(const oatpp::Int32& id) {
    auto dbResult = entryDb->getEntryById(id);
    checkDbResult(dbResult);
    if (!dbResult->hasMoreToFetch()) {
        return nullptr;
    }

    auto result = dbResult->fetch<oatpp::Vector<oatpp::Object<EntryDto>>>();
    if (result->size() != 1) {
        throw DbException("There are multiple entries with same id");
    }

    return result[0];
}

void EntryService::deleteEntryById(const oatpp::Int32& id) {
    auto dbResult = entryDb->deleteEntry(id);
    checkDbResult(dbResult);
}

oatpp::Object<PageDto<oatpp::Object<EntryDto>>> EntryService::getEntries(oatpp::Int32& pageNumber, oatpp::Int32& limit) {
    pageNumber = pageNumber ? pageNumber : oatpp::Int32(DEFAULT_PAGE_NUMBER);
    limit = limit ? limit : oatpp::Int32(DEFAULT_LIMIT);

    oatpp::Int32 offset = (pageNumber - 1) * limit;
    auto dbResult = entryDb->getEntries(limit, offset);
    checkDbResult(dbResult);

    auto result = dbResult->fetch<oatpp::Vector<oatpp::Object<EntryDto>>>();
    auto page = PageDto<oatpp::Object<EntryDto>>::createShared();
    page->limit = limit;
    page->pageNumber = pageNumber;
    page->items = result;
    page->count = result->size();

    return page;
}

void EntryService::checkDbResult(const std::shared_ptr<oatpp::orm::QueryResult> &res)
{
    if (!res->isSuccess()) {
        oatpp::String error = res->getErrorMessage();
        char* msg = new char[error->size()];
        memcpy(msg, error->c_str(), error->size());
        throw DbException(msg);
    }   
}