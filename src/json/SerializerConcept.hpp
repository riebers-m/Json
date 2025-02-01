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
        void serialize(std::string &stream, std::filesystem::path const &value);

        template<typename T>
        void serialize(std::string &stream, std::optional<T> const &value);

        template<typename T>
        void serialize(std::string &stream, std::vector<T> const value);

        JsonError deserialize(Tokens &, int &value);
        JsonError deserialize(Tokens &, float &value);
        JsonError deserialize(Tokens &, double &value);
        JsonError deserialize(Tokens &, std::string &value);
        JsonError deserialize(Tokens &, std::filesystem::path &value);
        JsonError deserialize(Tokens &, bool &value);
        template<typename T>
        JsonError deserialize(Tokens &, std::vector<T> &value);

        template<typename T>
        JsonError deserialize(Tokens &, std::optional<T> &value);
    } // namespace detail


} // namespace json

// Concepts

template<typename T>
concept OutSerializable = requires(std::string &stream, T val) { json::detail::serialize(stream, val); };

template<typename T>
concept InSerializable = requires(json::Tokens &tokens, T val) { json::detail::deserialize(tokens, val); };
template<typename T>
concept Serializable = InSerializable<T> && OutSerializable<T>;
