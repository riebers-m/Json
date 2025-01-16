//
// Created by Riebers on 05.01.2025.
//

#pragma once
#include <bitset>
#include <functional>
#include <string_view>

enum TypeFlags {
    TypeFlags_IsFundamental,
    TypeFlags_IsIntegral,
    TypeFlags_IsFloatingPoint,
    TypeFlags_IsTriviallyCopyable,

    TypeFlags_SIZE,
};

struct TypeInfo final {
    std::string_view name;
    std::uint32_t size;
    std::uint32_t alignment;

    std::bitset<TypeFlags_SIZE> flags{};

    std::function<void(std::string &, const void *)> serializer{};
    std::function<void(std::string &, void *)> deserializer{};

    template<typename T>
    static constexpr TypeInfo create();
};
