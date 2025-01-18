//
// Created by Riebers on 16.01.2025.
//

#pragma once
#include "MemberId.hpp"
#include "SerializerConcept.hpp"
#include "TypeId.hpp"

namespace json {
    namespace detail {

        inline void serialize(std::string &stream, int value) { stream += std::to_string(value); }
        inline void serialize(std::string &stream, bool value) { stream += value ? "true" : "false"; }
        inline void serialize(std::string &stream, float value) { stream += std::to_string(value); };
        inline void serialize(std::string &stream, double value) { stream += std::to_string(value); };
        inline void serialize(std::string &stream, std::string const &value) {
            stream += std::format("\"{}\"", value);
        };

        template<typename T>
        void serialize(std::string &stream, std::optional<T> const &value) {
            if (value) {
                serialize(stream, *value);
            } else {
                stream += "null";
            }
        }

        template<typename T>
        void serialize(std::string &stream, std::vector<T> const &value) {
            stream += "[";
            int items{};
            for (auto const &item: value) {
                if (items++ > 0) {
                    stream += ", ";
                }
                serialize(stream, item);
            }
            stream += "]";
        }

        inline void *offset_void_pointer(void *data, std::size_t offset) {
            return static_cast<uint8_t *>(data) + offset;
        }
        inline void const *offset_void_pointer(const void *data, std::size_t offset) {
            return static_cast<const uint8_t *>(data) + offset;
        }

        inline void serialize_type(std::string &stream, const void *data, TypeId type_id) {
            if (auto const &info = type_id.get_info(); info) {

                if (info.value().serializer) {
                    info.value().serializer(stream, data);
                } else {
                    stream += "{";
                    int member_count{};
                    for (auto &member: MemberId::get_member_infos(type_id)) {
                        if (!member.variable_id.is_ref_or_pointer()) { // why check this?
                            if (member_count++ != 0) {
                                stream += ",";
                            }
                            stream += "\"" + member.name + "\":";
                            serialize_type(stream, offset_void_pointer(data, member.offset),
                                           member.variable_id.get_type_id());
                        }
                    }
                    stream += "}";
                }
            } else {
                throw std::runtime_error("Serialization type not recognized");
            }
        }

        inline JsonError deserialize(Token const &token, int &value) {
            if (token.type != json::TokenType::Number) {
                return JsonError{Error::type_mismatch, 0};
            }
            value = std::stoi(token.value);
            return JsonError{Error::ok, 0};
        }

        inline JsonError deserialize(Token const &token, std::string &value) {
            if (token.type != json::TokenType::String) {
                return JsonError{Error::type_mismatch, 0};
            }
            value = token.value;
            return JsonError{Error::ok, 0};
        }

        inline JsonError deserialize_type(Tokens &tokens, void *data, TypeId type_id) {
            if (auto const &info = type_id.get_info(); info) {
                if (info.value().deserializer) {
                    auto const token = tokens.back();
                    tokens.pop_back();
                    return info.value().deserializer(token, data);
                } else {
                    auto token = tokens.back();
                    tokens.pop_back();
                    if (token.type != json::TokenType::LeftBrace) {
                        return JsonError{Error::braces_missmatch, 0};
                    }
                    auto &members = MemberId::get_member_infos(type_id);
                    token = tokens.back();
                    tokens.pop_back();

                    while (token.type != json::TokenType::RightBrace) {
                        std::string member_name;
                        if (auto const result = deserialize(token, member_name); result.error != Error::ok) {
                            return result;
                        }
                        auto const member = std::ranges::find(members, member_name, &MemberId::MemberInfo::name);
                        if (member != members.end() && !member->variable_id.is_ref_or_pointer()) {
                            token = tokens.back();
                            tokens.pop_back();

                            if (token.type != json::TokenType::KeyValueSeparator) {
                                return JsonError{Error::type_mismatch, 0};
                            }

                            if (auto const result =
                                        deserialize_type(tokens, offset_void_pointer(data, member->offset),
                                                         member->variable_id.get_type_id());
                                result.error != Error::ok) {
                                return result;
                            }
                        }
                        token = tokens.back();
                        tokens.pop_back();
                    }
                }
                return JsonError{Error::ok, 0};
            }
            return JsonError{Error::invalid_type, 0};
        }
    } // namespace detail

    template<typename T>
    std::string serialize_type(T const &value) {
        std::string stream;
        detail::serialize_type(stream, &value, TypeId::create<std::remove_cvref_t<T>>());
        return stream;
    }

    template<typename T>
    JsonError deserialize_type(std::string const &stream, T &value) {
        json::detail::Tokenizer tokenizer;
        if (auto const result = tokenizer.tokenize(stream); result.has_value()) {
            auto tokens = result.value();
            std::ranges::reverse(tokens.begin(), tokens.end());
            return detail::deserialize_type(tokens, &value, TypeId::create<std::remove_cvref_t<T>>());
        } else {
            return result.error();
        }
    }

} // namespace json
