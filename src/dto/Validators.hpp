#ifndef Validators_hpp
#define Validators_hpp

#include <regex>

#include "oatpp/web/protocol/http/outgoing/Response.hpp"
#include "oatpp/web/protocol/http/Http.hpp"

typedef oatpp::web::protocol::http::outgoing::Response OutgoingResponse;
typedef oatpp::web::protocol::http::Status Status;

namespace Validators {

    class EntryValidator {
        static inline std::string phonePattern = "\\+375\\d{9}";

    public:
        static void nonNull(const oatpp::Void& polymorph) {
            auto dispatcher = static_cast<const oatpp::data::type::__class::AbstractObject::PolymorphicDispatcher*>(polymorph.getValueType()->polymorphicDispatcher);
            auto fields = dispatcher->getProperties()->getList();
            auto dto = static_cast<oatpp::DTO*>(polymorph.get());
        
            for (auto const& field : fields) {

                if (field->name == "id")
                    continue;

                auto value = field->get(dto);

                OATPP_ASSERT_HTTP(value, Status::CODE_400, oatpp::String("Field '" + field->name + "' is null."));
            }
        }

        static void validate(const oatpp::Object<EntryDto>& dto) {
            nonNull(dto);
            
            OATPP_ASSERT_HTTP(std::regex_match(dto->phone->c_str(), std::regex(phonePattern)), Status::CODE_400, "Phone should be in '+375XXXXXXXXX' format.");
        }
    };
}

#endif