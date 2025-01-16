//
// Created by Riebers on 12.01.2025.
//

#pragma once
#include "Json.hpp"
#include "examples/MemberId.hpp"
#include "examples/TypeId.hpp"

namespace json {


    inline void serialize(JsonContainer &json, [[maybe_unused]] const void *data, TypeId type_id) {
        // auto const &info = type_id.get_info();

        auto const &members = MemberId::get_member_infos(type_id);

        for (auto const &member: members) {
            to_json(json, member);
        }
    }

    template<typename T>
    void serialize(JsonContainer &json, T &data) {
        serialize(json, data, TypeId::create<std::remove_cvref_t<T>>());
    }
} // namespace json
