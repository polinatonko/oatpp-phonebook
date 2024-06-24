#ifndef SWAGGER_COMPONENT_HPP
#define SWAGGER_COMPONENT_HPP

#include "oatpp-swagger/Model.hpp"
#include "oatpp-swagger/Resources.hpp"
#include "oatpp-swagger/ControllerPaths.hpp"
#include "oatpp/macro/component.hpp"

/**
 *  Configure Swagger info and resources paths
 */
class SwaggerComponent {
public:

    /**
     * General API docs info
     */
    OATPP_CREATE_COMPONENT(std::shared_ptr<oatpp::swagger::DocumentInfo>, swaggerDocumentInfo) ([] {

        oatpp::swagger::DocumentInfo::Builder builder;
        builder
        .setTitle("Phonebook service")
        .setDescription("Phonebook CRUD API project with Swagger docs")
        .setVersion("1.0")
        .setContactName("Polina Tonko")
        
        .addServer("http://localhost:8000", "server on localhost");

        return builder.build();

    } ());

    /**
     * Swagger UI resources
     */
    OATPP_CREATE_COMPONENT(std::shared_ptr<oatpp::swagger::Resources>, swaggerResources) ([] {

        return oatpp::swagger::Resources::loadResources(OATPP_SWAGGER_RES_PATH);

    } ());
};

#endif /* SWAGGER_COMPONENT_HPP */