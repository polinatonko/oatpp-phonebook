#ifndef ENTRY_DB_HPP
#define ENTRY_DB_HPP

#include "oatpp-sqlite/orm.hpp"
#include "oatpp/base/Log.hpp"

#include "dto/DTOs.hpp"

#include OATPP_CODEGEN_BEGIN(DbClient)

class EntryDb : public oatpp::orm::DbClient {
public:

    EntryDb(const std::shared_ptr<oatpp::orm::Executor>& executor)
        : oatpp::orm::DbClient(executor)
    {
        oatpp::orm::SchemaMigration migration(executor);
        migration.addFile(1, DATABASE_MIGRATIONS "/001_init.sql");
        migration.migrate();

        auto version = migration.getSchemaVersion();
        OATPP_LOGd("MyApp:EntryDb", "Migrations - OK, version={}.", version);
    }

    QUERY(createEntry,
        "INSERT INTO EntryDb"
        "(name, phone, address) VALUES "
        "(:entry.name, :entry.phone, :entry.address);",
        PARAM(oatpp::Object<EntryDto>, entry))

    QUERY(updateEntry,
        "UPDATE EntryDb "
        "SET "
        "name=:entry.name,"
        "phone=:entry.phone,"
        "address=:entry.address "
        "WHERE "
        "id=:entry.id;",
        PARAM(oatpp::Object<EntryDto>, entry))

    QUERY(getEntryById,
        "SELECT * FROM EntryDb WHERE id=:id;",
        PARAM(oatpp::Int32, id))
        
    QUERY(deleteEntry,
        "DELETE FROM EntryDb WHERE id=:id;",
        PARAM(oatpp::Int32, id))

    QUERY(getEntries,
        "SELECT * FROM EntryDb LIMIT :limit OFFSET :offset;",
        PARAM(oatpp::Int32, limit),
        PARAM(oatpp::Int32, offset))
    
};

#include OATPP_CODEGEN_END(DbClient)

#endif /* ENTRY_DB_HPP */