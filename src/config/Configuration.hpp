#ifndef CONFIGURATION_HPP
#define CONFIGURATION_HPP

#include "oatpp/Types.hpp"
#include "oatpp/base/Log.hpp"
#include "oatpp/json/ObjectMapper.hpp"

/**
 * Configuration file parser.
 */
class Configuration {

    bool parse(const oatpp::String& config) {
        if (!config) {
            OATPP_LOGe("Server:Configuration", "Load config file error");
            return false;
        }

        auto json = std::make_shared<oatpp::json::ObjectMapper>();
        auto fields = json->readFromString<oatpp::Fields<oatpp::Any>>(config);

        auto threadsConfig = fields["thread"].retrieve<oatpp::Fields<oatpp::Any>>();
        if (!threadsConfig) {
            OATPP_LOGe("Server:Configuration", "Thread config error");
            exit(1);
        }

        dataThread = threadsConfig["dataThread"].retrieve<oatpp::Int64>().getValue(4);
        ioThread = threadsConfig["ioThread"].retrieve<oatpp::Int64>().getValue(1);
        timerThread = threadsConfig["timerThread"].retrieve<oatpp::Int64>().getValue(1);

        auto dbConfig = fields["db"].retrieve<oatpp::Fields<oatpp::Any>>();
        if (!dbConfig) {
            OATPP_LOGe("Server:Configuration", "Db config error");
            exit(1);
        }

        dbPool = dbConfig["pool"].retrieve<oatpp::Int64>().getValue(10);
        dbTtl = dbConfig["ttl"].retrieve<oatpp::Int64>().getValue(5);

        auto serverConfig = fields["server"].retrieve<oatpp::Fields<oatpp::Any>>();
        if (!serverConfig) {
            OATPP_LOGe("Server:Configuration", "Server config error");
            exit(1);
        }

        serverAddress = serverConfig["address"].retrieve<oatpp::String>();
        serverPort = serverConfig["port"].retrieve<oatpp::Int64>().getValue(8000);
        
        return true;
    }

public:
    /**
     * Constructor with config filename.
     * @param filename - name of configuration file.
     */
    Configuration(const char* filename) {
        oatpp::String config = oatpp::String::loadFromFile(filename);
        if (!parse(config)) {
            exit(1);
        }
    }

    oatpp::String serverAddress;
    v_uint16 serverPort, dbPool, dbTtl;
    int dataThread, ioThread, timerThread;
};

#endif /* CONFIGURATION_HPP */