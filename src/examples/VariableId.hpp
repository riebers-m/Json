//
// Created by Riebers on 11.01.2025.
//
#include <cstdint>
#include <string>

#include "TypeId.hpp"

#pragma once

class VariableId {
private:
    static constexpr std::uint32_t const_flag = 1 << 0;
    static constexpr std::uint32_t reference_flag = 1 << 1;
    static constexpr std::uint32_t volatile_flag = 1 << 2;
    static constexpr std::uint32_t rvalue_reference_flag = 1 << 3;

    TypeId m_id{};
    std::uint32_t array_size{};
    std::uint16_t pointer_amount{};
    std::uint8_t trait_flags{};

public:
    VariableId() = default;
    explicit VariableId(TypeId const &id) : m_id(id) {}

    [[nodiscard]] bool constexpr is_const() const { return trait_flags & const_flag; }
    [[nodiscard]] bool constexpr is_reference() const { return trait_flags & reference_flag; }
    [[nodiscard]] bool constexpr is_volatile() const { return trait_flags & volatile_flag; }
    [[nodiscard]] bool constexpr is_rvalue_reference() const { return trait_flags & rvalue_reference_flag; }
    [[nodiscard]] bool constexpr is_pointer() const { return pointer_amount; }
    [[nodiscard]] bool constexpr is_array() const { return pointer_amount; }
    [[nodiscard]] bool constexpr is_ref_or_pointer() const {
        return is_pointer() || is_reference() || is_rvalue_reference();
    }

    constexpr void set_const_flag() { trait_flags |= const_flag; }
    constexpr void set_reference_flag() { trait_flags |= reference_flag; }
    constexpr void set_volatile_flag() { trait_flags |= volatile_flag; }
    constexpr void set_rvalue_reference_flag() { trait_flags |= rvalue_reference_flag; }

    constexpr void remove_const_flag() { trait_flags &= ~const_flag; }
    constexpr void remove_reference_flag() { trait_flags &= ~reference_flag; }
    constexpr void remove_volatile_flag() { trait_flags &= ~volatile_flag; }
    constexpr void remove_rvalue_reference_flag() { trait_flags &= ~rvalue_reference_flag; }

    [[nodiscard]] constexpr TypeId const &get_type_id() const { return m_id; }
    [[nodiscard]] constexpr std::uint32_t get_array_size() const { return array_size; }
    [[nodiscard]] constexpr std::uint16_t get_pointer_amount() const { return pointer_amount; }

    constexpr void set_id(TypeId const &id) { m_id = id; }
    constexpr void set_array_size(std::uint32_t size) { array_size = size; }
    constexpr void set_pointer_amount(std::uint16_t size) { pointer_amount = size; }

    [[nodiscard]] constexpr std::uint32_t get_size() const {
        return is_ref_or_pointer() ? sizeof(void *) : get_type_id().get_size().value_or(0) * get_array_size();
    }

    [[nodiscard]] constexpr std::uint32_t get_alignment() const {
        return is_ref_or_pointer() ? alignof(void *) : get_type_id().get_alignment().value_or(0);
    }

    [[nodiscard]] constexpr std::uint64_t get_hash() const {
        return m_id.get_id() ^ array_size ^ (static_cast<uint64_t>(pointer_amount) << 32) ^
               (static_cast<uint64_t>(trait_flags) << 40);
    }

    template<typename T>
    static constexpr VariableId create();

    friend bool operator==(const VariableId &lhs, const VariableId &rhs);
};

template<typename T>
constexpr std::uint32_t count_pointers(std::uint32_t counter = 0) {
    if constexpr (std::is_pointer_v<T>) {
        return count_pointers<std::remove_pointer_t<T>>(counter);
    } else {
        return counter;
    }
}

template<typename T>
struct remove_all_pointers {
    using Type = T;
};

template<typename T>
struct remove_all_pointers<T *> {
    using Type = typename remove_all_pointers<T>::Type;
};
// std::remove_pointer only removes one * not if type has pointer to pointer **
template<typename T>
using remove_all_pointers_t = typename remove_all_pointers<T>::Type;

template<typename T>
inline constexpr VariableId VariableId::create() {
    // to get underlying type, e.g. if float a[1][2][3] you get float type
    using Type_RemovedExtents = std::remove_all_extents_t<T>;
    // if you have a reference, e.g. int& pr int&& you get int type
    // does nothing on pointer, e.g. int*
    using Type_RemovedRefs = std::remove_reference_t<Type_RemovedExtents>;
    using Type_RemovedPointers = remove_all_pointers_t<Type_RemovedRefs>;

    using StrippedType = std::remove_cvref_t<Type_RemovedPointers>;
    RegisterType<StrippedType> TypeRegister{};

    constexpr bool is_const{std::is_const_v<T>};
    constexpr bool is_volatile{std::is_volatile_v<T>};
    constexpr bool is_ref{std::is_reference_v<T>};
    constexpr bool is_rvalue_ref{std::is_rvalue_reference_v<T>};

    constexpr std::uint32_t pointer_amount{count_pointers<Type_RemovedRefs>()};

    auto variable = VariableId{TypeId::create<StrippedType>()};

    if constexpr (is_const) {
        variable.set_const_flag();
    }
    if constexpr (is_volatile) {
        variable.set_volatile_flag();
    }
    if constexpr (is_ref) {
        variable.set_reference_flag();
    }
    if constexpr (is_rvalue_ref) {
        variable.set_rvalue_reference_flag();
    }

    variable.set_pointer_amount(pointer_amount);

    if constexpr (!std::is_same_v<void, Type_RemovedExtents>) {
        constexpr std::uint32_t array_size{sizeof(T) / sizeof(Type_RemovedExtents)};
        variable.set_array_size(array_size);
    } else {
        variable.set_array_size(1);
    }
    return variable;
}

template<>
struct std::hash<VariableId> {
    std::size_t operator()(VariableId const &id) const noexcept {
        return static_cast<std::size_t>(id.get_type_id().get_id());
    }
};

inline bool operator==(VariableId const &lhs, VariableId const &rhs) {
    return lhs.m_id == rhs.m_id && lhs.array_size == rhs.array_size && lhs.pointer_amount == rhs.pointer_amount &&
           lhs.trait_flags == rhs.trait_flags;
}

inline std::string const &get_variable_name(VariableId const &id) {
    static std::unordered_map<VariableId, std::string> variable_name_map{};

    if (const auto it = variable_name_map.find(id); it != variable_name_map.end()) {
        return it->second;
    }
    auto name = std::string{id.get_type_id().get_name().value_or("<unknown>")};
    if (id.is_volatile()) {
        name = "volatile " + name;
    }
    if (id.is_const()) {
        name = "const " + name;
    }

    std::uint32_t pointer_amount = id.get_pointer_amount();
    for (std::uint32_t i{}; i < pointer_amount; ++i) {
        name += "*";
    }

    if (id.get_array_size() > 1) {
        name += "[" + std::to_string(id.get_array_size()) + "]";
    }
    if (id.is_rvalue_reference()) {
        name += "&&";
    }
    if (id.is_reference()) {
        name += "&";
    }

    return variable_name_map.try_emplace(id, std::move(name)).first->second;
}
