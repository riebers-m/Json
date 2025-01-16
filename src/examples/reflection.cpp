//
// Created by Riebers on 05.01.2025.
//

#include <iostream>
#include <typeinfo>
#include "MemberId.hpp"
#include "Serializer.hpp"
#include "TypeHash.hpp"

// struct Vector {
//     float X{}, Y{}, Z{};
// };
//
// REGISTER_TYPE(Vector);
// REGISTER_MEMBER(Vector, X);
// REGISTER_MEMBER(Vector, Y);
// REGISTER_MEMBER(Vector, Z);
//
// struct Quaternion {
//     float X{}, Y{}, Z{}, W{};
// };
//
// REGISTER_TYPE(Quaternion);
// REGISTER_MEMBER(Quaternion, X);
// REGISTER_MEMBER(Quaternion, Y);
// REGISTER_MEMBER(Quaternion, Z);
// REGISTER_MEMBER(Quaternion, W);
//
// #pragma warning(disable : 4324) // disable padding warning
// struct Transform {
//     alignas(16) Vector Translation{};
//     alignas(16) Quaternion Rotation{};
//     alignas(16) Vector Scale{};
// };
// #pragma warning(default : 4324)
//
// REGISTER_TYPE(Transform);
// REGISTER_MEMBER(Transform, Translation);
// REGISTER_MEMBER(Transform, Rotation);
// REGISTER_MEMBER(Transform, Scale);
//
// class GameObject {
// private:
//     std::string Name{};
//     uint32_t Id{};
//     Transform Transform{};
//
//     friend struct RegisterGameObject;
// };
//
// struct RegisterGameObject {
//     REGISTER_MEMBER(GameObject, Name);
//     REGISTER_MEMBER(GameObject, Id);
//     REGISTER_MEMBER(GameObject, Transform);
// };
//
void PrintTypeInfo(TypeId id) {
    std::cout << "Type: " << id.get_name().value_or("<unknown>") << '\n';
    std::cout << "Size: " << id.get_size().value_or(0) << '\n';
    std::cout << "Alignment: " << id.get_alignment().value_or(0) << '\n';

    std::cout << "Member variables:\n";
    for (auto &info: MemberId::get_member_infos(id)) {
        std::cout << '\t' << get_variable_name(info.variable_id) << " [" << info.name << "] At Offset ["
                  << info.offset << "]\n";
    }
    std::cout << "\n\n";
}

struct Vector {
    int x{};
    int y{};
    int z{};
};

REGISTER_MEMBER(Vector, x);
REGISTER_MEMBER(Vector, y);
REGISTER_MEMBER(Vector, z);

struct Test {
    Vector vec{};
    int y{};
    bool z{false};
    float a{1.23};
    double b{23.23};
    std::string str{"Hello, World!"};
    std::optional<int> opt;
    std::vector<float> v{1.23};
    std::vector<std::optional<std::string>> v2{};
};
REGISTER_MEMBER(Test, vec);
REGISTER_MEMBER(Test, y);
REGISTER_MEMBER(Test, z);
REGISTER_MEMBER(Test, a);
REGISTER_MEMBER(Test, b);
REGISTER_MEMBER(Test, str);
REGISTER_MEMBER(Test, opt);
REGISTER_MEMBER(Test, v);
REGISTER_MEMBER(Test, v2);

int main() {
    Test test{.vec = {}, .y = 5, .opt = 35, .v = {1.2, 2.3, 3.4, 4.5}, .v2 = {"a", "b", std::nullopt}};
    // // int x = 5;
    auto const stream = json::serialize_type(test);
    //
    // PrintTypeInfo(TypeId::create<Test>());
    std::cout << stream << '\n';
    // PrintTypeInfo(TypeId::create<Quaternion>());
    // PrintTypeInfo(TypeId::create<Transform>());
    // PrintTypeInfo(TypeId::create<GameObject>());
}
