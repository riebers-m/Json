//
// Created by Riebers on 10.01.2025.
//
#include <cstdint>
#include <optional>
#include <unordered_map>

#include "TypeInfo.hpp"

#pragma once

class TypeId {
private:
    using Id = std::uint64_t;
    using TypeContainer = std::unordered_map<Id, TypeInfo>;
    Id id;
    static inline TypeContainer types;

public:
    explicit TypeId(Id id) : id(id) {}
    TypeId() = default;


    [[nodiscard]] bool is_fundamental() const { return get_info()->flags[TypeFlags_IsFundamental]; }
    [[nodiscard]] bool is_integral() const { return get_info()->flags[TypeFlags_IsIntegral]; }
    [[nodiscard]] bool is_floating_point() const { return get_info()->flags[TypeFlags_IsFloatingPoint]; }
    [[nodiscard]] bool is_triviallycopyable() const { return get_info()->flags[TypeFlags_IsTriviallyCopyable]; }

    [[nodiscard]] constexpr Id get_id() const { return id; }
    [[nodiscard]] std::optional<TypeInfo> get_info() const {
        if (auto item = get_container().find(id); item != get_container().end()) {
            return item->second;
        }
        return std::nullopt;
    }
    [[nodiscard]] std::optional<std::string_view> get_name() const {
        if (auto item = get_container().find(id); item != get_container().end()) {
            return item->second.name;
        }
        return std::nullopt;
    }

    [[nodiscard]] std::optional<std::uint32_t> get_size() const {
        if (auto item = get_container().find(id); item != get_container().end()) {
            return item->second.size;
        }
        return std::nullopt;
    }

    [[nodiscard]] std::optional<std::uint32_t> get_alignment() const {
        if (auto item = get_container().find(id); item != get_container().end()) {
            return item->second.alignment;
        }
        return std::nullopt;
    }

    static TypeContainer &get_container() { return types; }

    template<typename T>
    static TypeId create();
};

constexpr bool operator==(const TypeId lhs, const TypeId rhs) { return lhs.get_id() == rhs.get_id(); }

template<typename T>
class RegisterType {
private:
    class RegisterOnce {
    public:
        RegisterOnce() {
            auto const info = TypeInfo::create<T>();
            auto const id = TypeId::create<T>();

            TypeId::get_container().try_emplace(id.get_id(), info);
        }
    };
    inline static RegisterOnce registerOnce{};
};

#define REGISTER_TYPE(TYPE) RegisterType<TYPE> RegisterType_##TYPE

template<>
struct std::hash<TypeId> {
    std::size_t operator()(const TypeId &id) const noexcept { return static_cast<size_t>(id.get_id()); }
};

template<typename T>
constexpr TypeInfo TypeInfo::create() {
    RegisterType<T> RegisterType{};

    TypeInfo info{};

    info.name = Reflection::TypeName<T>();

    if constexpr (!std::is_same_v<T, void>) {
        info.size = sizeof(T);
        info.alignment = alignof(T);
    }

    info.flags[TypeFlags_IsFundamental] = std::is_fundamental_v<T>;
    info.flags[TypeFlags_IsIntegral] = std::is_integral_v<T>;
    info.flags[TypeFlags_IsFloatingPoint] = std::is_floating_point_v<T>;
    info.flags[TypeFlags_IsTriviallyCopyable] = std::is_trivially_copyable_v<T>;

    return info;
}

template<typename T>
TypeId TypeId::create() {
    RegisterType<T> RegisterType{};

    return TypeId{Reflection::TypeId<T>()};
}
