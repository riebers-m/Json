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

        template<typename T>
        Error deserialize_type(std::string const &stream, T &value) {
            return Error::ok;
        }

    } // namespace detail

    template<typename T>
    std::string serialize_type(T const &value) {
        std::string stream;
        detail::serialize_type(stream, &value, TypeId::create<std::remove_cvref_t<T>>());
        return stream;
    }


} // namespace json
