//
// Created by Riebers on 16.01.2025.
//

#pragma once
#include "MemberId.hpp"
#include "SerializerConcept.hpp"
#include "Tokenizer.hpp"
#include "TypeId.hpp"

namespace json {
    namespace detail {

        inline void serialize(std::string &stream, int value) { stream += std::to_string(value); }
        inline void serialize(std::string &stream, bool value) { stream += value ? "true" : "false"; }
        inline void serialize(std::string &stream, float value) { stream += std::to_string(value); };
        inline void serialize(std::string &stream, double value) { stream += std::to_string(value); };
        inline void serialize(std::string &stream, std::size_t value) { stream += std::to_string(value); }
        inline void serialize(std::string &stream, std::string const &value) {
            stream += std::format("\"{}\"", value);
        };
        inline void serialize(std::string &stream, std::filesystem::path const &value) {
            stream += value.string();
        }

        template<typename T>
        void serialize(std::string &stream, std::optional<T> const &value) {
            if (value) {
                serialize(stream, *value);
            } else {
                stream += "null";
            }
        }


        inline void *offset_void_pointer(void *data, std::size_t offset) {
            return static_cast<uint8_t *>(data) + offset;
        }
        inline void const *offset_void_pointer(const void *data, std::size_t offset) {
            return static_cast<const uint8_t *>(data) + offset;
        }

        inline Token next_token(Tokens &tokens) {
            auto const next_token = tokens.back();
            tokens.pop_back();
            return next_token;
        }

        inline void skip_seperator_and_value(Tokens &tokens) {
            tokens.pop_back();
            tokens.pop_back();
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

        template<typename T>
        void serialize(std::string &stream, std::vector<T> const value) {
            stream += "[";
            int items{};
            for (auto const &item: value) {
                if (items++ > 0) {
                    stream += ", ";
                }
                serialize_type(stream, &item, TypeId::create<T>());
            }
            stream += "]";
        }

        inline JsonError deserialize(Tokens &tokens, int &value) {
            auto const token = next_token(tokens);
            if (token.type != json::TokenType::Number) {
                return JsonError{Error::type_mismatch, 0};
            }
            value = std::stoi(token.value);
            return JsonError{Error::ok, tokens.size()};
        }
        inline JsonError deserialize(Tokens &tokens, float &value) {
            auto const token = next_token(tokens);
            if (token.type != TokenType::Number) {
                return JsonError{Error::type_mismatch, tokens.size()};
            }
            value = std::stof(token.value);
            return JsonError{Error::ok, 0};
        }
        inline JsonError deserialize(Tokens &tokens, double &value) {
            auto const token = next_token(tokens);
            if (token.type != TokenType::Number) {
                return JsonError{Error::type_mismatch, tokens.size()};
            }
            value = std::stod(token.value);
            return JsonError{Error::ok, 0};
        }

        inline JsonError deserialize(Tokens &tokens, std::size_t &value) {
            auto const token = next_token(tokens);
            if (token.type != TokenType::Number) {
                return JsonError{Error::type_mismatch, tokens.size()};
            }
            value = std::stoul(token.value);
            return JsonError{Error::ok, 0};
        }

        inline JsonError deserialize(Tokens &tokens, bool &value) {
            auto const token = next_token(tokens);
            if (token.type != TokenType::Boolean) {
                return JsonError{Error::type_mismatch, tokens.size()};
            }
            if (token.value.find("true") != std::string::npos) {
                value = true;
            } else {
                value = false;
            }
            return JsonError{Error::ok, 0};
        }

        template<typename T>
        JsonError deserialize(Tokens &tokens, std::optional<T> &value) {
            T data{};

            if (tokens.back().type == TokenType::Null) {
                value = std::nullopt;
                return JsonError{Error::ok, 0};
            }
            if (auto const result = deserialize(tokens, data); result.error != Error::ok) {
                return result;
            }
            value = data;
            return JsonError{Error::ok, 0};
        }

        inline JsonError deserialize(Tokens &tokens, std::string &value) {
            auto const token = next_token(tokens);
            if (token.type != json::TokenType::String) {
                return JsonError{Error::type_mismatch, 0};
            }
            value = token.value;
            return JsonError{Error::ok, 0};
        }

        inline JsonError deserialize(Tokens &tokens, std::filesystem::path &value) {
            auto const token = next_token(tokens);
            if (token.type != json::TokenType::String) {
                return JsonError{Error::type_mismatch, 0};
            }
            value = token.value;
            return JsonError{Error::ok, 0};
        }

        inline JsonError deserialize_type(Tokens &tokens, void *data, TypeId type_id) {
            if (auto const &info = type_id.get_info(); info) {
                if (info.value().deserializer) {
                    return info.value().deserializer(tokens, data);
                }

                auto token = next_token(tokens);
                if (token.type != TokenType::LeftBrace) {
                    return JsonError{Error::braces_missmatch, 0};
                }
                auto &members = MemberId::get_member_infos(type_id);

                while (token.type != TokenType::RightBrace) {
                    std::string member_name;
                    if (auto const result = deserialize(tokens, member_name); result.error != Error::ok) {
                        return result;
                    }
                    auto const member = std::ranges::find(members, member_name, &MemberId::MemberInfo::name);
                    if (member != members.end() && !member->variable_id.is_ref_or_pointer()) {
                        token = next_token(tokens);

                        if (token.type != json::TokenType::KeyValueSeparator) {
                            return JsonError{Error::type_mismatch, 0};
                        }

                        if (auto const result = deserialize_type(tokens, offset_void_pointer(data, member->offset),
                                                                 member->variable_id.get_type_id());
                            result.error != Error::ok) {
                            return result;
                        }
                    } else {
                        skip_seperator_and_value(tokens);
                    }
                    token = next_token(tokens);
                }
                return JsonError{Error::ok, 0};
            }
            return JsonError{Error::invalid_type, 0};
        }

        template<typename T>
        JsonError deserialize(Tokens &tokens, std::vector<T> &value) {
            auto token = next_token(tokens);
            if (token.type != TokenType::LeftBracket) {
                return JsonError{Error::braces_missmatch, 0};
            }
            while (token.type != TokenType::RightBracket) {
                T data{};
                if (auto const result = detail::deserialize_type(tokens, &data, TypeId::create<T>());
                    result.error == Error::ok) {
                    value.emplace_back(data);
                } else {
                    return result;
                }
                token = next_token(tokens);
                if (token.type != TokenType::Comma && token.type != TokenType::RightBracket) {
                    return JsonError{Error::missing_comma, 0};
                }
            }
            return JsonError{Error::ok, 0};
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
            if (auto tokens = result.value(); !tokens.empty()) {
                std::ranges::reverse(tokens.begin(), tokens.end());
                return detail::deserialize_type(tokens, &value, TypeId::create<std::remove_cvref_t<T>>());
            }
            return {Error::ok, 0};
        } else {
            return result.error();
        }
    }


} // namespace json
