#ifndef DB_COMPONENT_HPP
#define DB_COMPONENT_HPP

#include "oatpp/macro/component.hpp"

#include "db/EntryDb.hpp"

class DbComponent {
public:

    /**
     * Create db connection provider component
     */
    OATPP_CREATE_COMPONENT(std::shared_ptr<oatpp::provider::Provider<oatpp::sqlite::Connection>>, connectionProvider) ([] {
        
        /* Create database-specific ConnectionProvider */
        auto connectionProvider = std::make_shared<oatpp::sqlite::ConnectionProvider>(DATABASE_FILE);

        /* Create database-specific ConnectionPool */
        return oatpp::sqlite::ConnectionPool::createShared(connectionProvider,
                                                       10 /* max-connections */,
                                                       std::chrono::seconds(5) /* connection TTL */);
    } ());

    /**
     * Create database client
     */
    OATPP_CREATE_COMPONENT(std::shared_ptr<EntryDb>, entryDb) ([] {
        
        /* Get database ConnectionProvider component */
        OATPP_COMPONENT(std::shared_ptr<oatpp::provider::Provider<oatpp::sqlite::Connection>>, connectionProvider);
        
        /* Create db executor */
        auto executor = std::make_shared<oatpp::sqlite::Executor>(connectionProvider);

        /* Create db client */
        return std::make_shared<EntryDb>(executor);

    } ());
};

#endif /* DB_COMPONENT_HPP */