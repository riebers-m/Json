//
// Created by Riebers on 11.01.2025.
//

#pragma once
#include <cassert>
#include <set>
#include <stdexcept>


#include "TypeId.hpp"
#include "VariableId.hpp"

class MemberId {
public:
    struct MemberInfo {
        std::string name{};
        VariableId variable_id{};
        std::uint32_t offset{};
        std::uint32_t size{};
        std::uint32_t alignment{};

        constexpr bool operator<(const MemberInfo &other) const { return offset < other.offset; }
    };

private:
    TypeId id{};
    std::uint32_t offset{};

    struct StaticData {
        std::unordered_map<TypeId, std::set<MemberInfo>> member_infos{};
        std::unordered_map<TypeId, std::unordered_map<std::string, std::uint32_t>> members_infos_name_map{};
    };

    static StaticData &get_static_data() {
        static StaticData data{};
        return data;
    }

public:
    constexpr MemberId(TypeId id, std::uint32_t offset) : id{id}, offset{offset} {}

    [[nodiscard]] MemberInfo get_member_info() const;

    [[nodiscard]] constexpr TypeId get_type_id() const { return id; }
    [[nodiscard]] constexpr std::uint32_t get_offset() const { return offset; }
    [[nodiscard]] std::string const &get_name() const { return get_member_info().name; }
    [[nodiscard]] VariableId get_variable_id() const { return get_member_info().variable_id; }
    [[nodiscard]] std::uint32_t get_alignment() const { return get_member_info().alignment; }
    [[nodiscard]] std::uint32_t get_size() const { return get_member_info().size; }

    template<typename Class, typename Field>
    static MemberId RegisterField(std::string const &field_name, std::uint32_t field_offset);

    //    template<typename Field>
    //    static MemberId RegisterField(TypeId class_id, std::string const &field_name, std::uint32_t
    //    field_offset);

    static MemberId RegisterField(TypeId class_id, VariableId member_id, std::string const &field_name,
                                  std::uint32_t field_offset, std::uint32_t field_size,
                                  std::uint32_t field_alignment);

    static const std::set<MemberInfo> &get_member_infos(TypeId id) { return get_static_data().member_infos[id]; }
    static MemberInfo get_member_info(TypeId id, std::string const &field_name);

    static auto const &get_all_member_infos() { return get_static_data().member_infos; }
    static bool exists(TypeId class_id) { return get_static_data().member_infos.contains(class_id); }
};

struct RegisterMember final {
    RegisterMember(TypeId class_id, VariableId member_id, std::string const &field_name,
                   std::uint32_t field_offset, std::uint32_t field_size, std::uint32_t field_alignment) {
        MemberId::RegisterField(class_id, member_id, field_name, field_offset, field_size, field_alignment);
    }
};

#define REGISTER_MEMBER(TYPE, FIELD)                                                                              \
    inline RegisterType<TYPE> __##TYPE##FIELD;                                                                    \
    inline static RegisterMember TYPE##FIELD{TypeId::create<TYPE>(),                                              \
                                             VariableId::create<decltype(TYPE::FIELD)>(),                         \
                                             #FIELD,                                                              \
                                             offsetof(TYPE, FIELD),                                               \
                                             sizeof(decltype(TYPE::FIELD)),                                       \
                                             alignof(decltype(TYPE::FIELD))};

inline MemberId::MemberInfo MemberId::get_member_info() const {
    auto &member_infos = get_member_infos(id);

    MemberInfo info_to_find{};
    info_to_find.offset = offset;

    if (auto it = member_infos.find(info_to_find); it != member_infos.end()) {
        return *it;
    }
    throw std::runtime_error("MemberId::get_member_info() failed");
}


inline MemberId MemberId::RegisterField(TypeId class_id, VariableId member_id, std::string const &field_name,
                                        std::uint32_t field_offset, std::uint32_t field_size,
                                        std::uint32_t field_alignment) {
    MemberInfo info{};
    info.name = field_name;
    info.variable_id = member_id;
    info.alignment = field_alignment;
    info.offset = field_offset;
    info.size = field_size;

    get_static_data().member_infos[class_id].emplace(std::move(info));
    get_static_data().members_infos_name_map[class_id].try_emplace(field_name, field_offset);

    return {class_id, field_offset};
}

inline MemberId::MemberInfo MemberId::get_member_info(TypeId id, std::string const &field_name) {
    auto &member_infos = get_static_data().members_infos_name_map[id];
    assert(member_infos.contains(field_name));

    MemberId const member_id{id, member_infos[field_name]};
    return member_id.get_member_info();
}

template<typename Class, typename Field>
MemberId MemberId::RegisterField(std::string const &field_name, std::uint32_t field_offset) {
    auto register_class = RegisterType<Class>();
    auto register_field = RegisterType<Field>();

    return RegisterField(TypeId::create<Class>(), VariableId::create<Field>(), field_name, field_offset,
                         sizeof(Field), alignof(Field));
}

template<typename Field>
MemberId RegisterField(TypeId class_id, std::string const &field_name, std::uint32_t field_offset) {
    auto register_class = RegisterType<Field>();

    return RegisterField(class_id, VariableId::create<Field>(), field_name, field_offset, sizeof(Field),
                         alignof(Field));
}
