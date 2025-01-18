//
// Created by Riebers on 12.01.2025.
//

#pragma once
#include <string>
#include "Tokenizer.hpp"
namespace json {

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

        JsonError deserialize(Token const &, int &value);
        JsonError deserialize(Token const &, std::string &value);
    } // namespace detail


} // namespace json

// Concepts

template<typename T>
concept OutSerializable = requires(std::string &stream, T val) { json::detail::serialize(stream, val); };

template<typename T>
concept InSerializable = requires(json::Token const &token, T val) { json::detail::deserialize(token, val); };
template<typename T>
concept Serializable = InSerializable<T> && OutSerializable<T>;
