//
// Created by Riebers on 12.01.2025.
//

#pragma once
#include <string>

namespace json {

    enum class Error {
        ok,
        missing_comma,
        unquoted_keys,
        braces_missmatch,
        invalid_escape_sequence,
        trailing_comma,
        type_mismatch,

    };

    namespace detail {
        void serialize(std::string &stream, int value);
        void serialize(std::string &stream, bool value);
        void serialize(std::string &stream, float value);
        void serialize(std::string &stream, double value);
        void serialize(std::string &stream, std::string const &value);

        template<typename T>
        void serialize(std::string &stream, std::optional<T> const &value);

        template<typename T>
        void serialize(std::string &stream, std::vector<T> const &value);

        Error deserialize(std::string &stream, int value);
    } // namespace detail


} // namespace json

// Concepts

template<typename T>
concept InSerializable = requires(std::string &stream, T val) { json::detail::serialize(stream, val); };

template<typename T>
concept OutSerializable = requires(std::string &stream, T val) { json::detail::deserialize(stream, val); };
template<typename T>
concept Serializable = InSerializable<T>; // && OutSerializable<T>;
