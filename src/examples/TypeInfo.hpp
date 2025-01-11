//
// Created by Riebers on 05.01.2025.
//

#pragma once
#include <bitset>
#include <string_view>
#include "TypeHash.hpp"

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

    template<typename T>
    static constexpr TypeInfo create();
};
