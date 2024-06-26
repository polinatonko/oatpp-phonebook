#ifndef VALIDATORS_HPP
#define VALIDATORS_HPP

#include <regex>

#include "oatpp/web/protocol/http/outgoing/Response.hpp"
#include "oatpp/web/protocol/http/Http.hpp"

#include "errorHandler/Exception.hpp"

typedef oatpp::web::protocol::http::outgoing::Response OutgoingResponse;
typedef oatpp::web::protocol::http::Status Status;

namespace Validators {

    /**
     * Class which validates null fields of object.
     */
    class NonNullValidator {
    public:
        /**
         * Checks all fields of object for null values.
         * @param polymorph - object to check.
         */
        static void nonNull(const oatpp::Void& polymorph) {
            auto dispatcher = static_cast<const oatpp::data::type::__class::AbstractObject::PolymorphicDispatcher*>(polymorph.getValueType()->polymorphicDispatcher);
            auto fields = dispatcher->getProperties()->getList();
            auto dto = static_cast<oatpp::BaseObject*>(polymorph.get());
        
            for (auto const& field : fields) {
                if (field->name == "id")
                    continue;

                if (!field->get(dto)) {
                    char* message = new char[strlen(field->name.c_str()) + 26];
                    sprintf(message, "Field '%s' shouldn't be null", field->name.c_str());
                    throw ValidationException(message);
                }
            }
        }
    };

    /**
     *  Class which validates EntryRequestDto.
     * */
    class EntryValidator : public NonNullValidator {
        static inline std::string phonePattern = "\\+375\\d{9}";

    public:
        /**
         * Checks all fields of EntryRequestDto for null values and custom logic.
         * @param dto - EntryRequestDto object to check.
         */
        static void validate(const oatpp::Object<EntryRequestDto>& dto) {
            nonNull(dto);

            if (!std::regex_match(dto->phone->c_str(), std::regex(phonePattern))) {
                throw ValidationException("Phone should be in '+375XXXXXXXXX' format.");
            }
        }
    };
}

#endif /* VALIDATORS_HPP */