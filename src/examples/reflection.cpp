//
// Created by Riebers on 05.01.2025.
//

#include <iostream>
#include "MemberId.hpp"

struct Vector {
    float X{}, Y{}, Z{};
};

REGISTER_TYPE(Vector);
REGISTER_MEMBER(Vector, X);
REGISTER_MEMBER(Vector, Y);
REGISTER_MEMBER(Vector, Z);

struct Quaternion {
    float X{}, Y{}, Z{}, W{};
};

REGISTER_TYPE(Quaternion);
REGISTER_MEMBER(Quaternion, X);
REGISTER_MEMBER(Quaternion, Y);
REGISTER_MEMBER(Quaternion, Z);
REGISTER_MEMBER(Quaternion, W);

#pragma warning(disable : 4324) // disable padding warning
struct Transform {
    alignas(16) Vector Translation{};
    alignas(16) Quaternion Rotation{};
    alignas(16) Vector Scale{};
};
#pragma warning(default : 4324)

REGISTER_TYPE(Transform);
REGISTER_MEMBER(Transform, Translation);
REGISTER_MEMBER(Transform, Rotation);
REGISTER_MEMBER(Transform, Scale);

class GameObject {
private:
    std::string Name{};
    uint32_t Id{};
    Transform Transform{};

    friend struct RegisterGameObject;
};

struct RegisterGameObject {
    REGISTER_MEMBER(GameObject, Name);
    REGISTER_MEMBER(GameObject, Id);
    REGISTER_MEMBER(GameObject, Transform);
};

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

int main() {
    PrintTypeInfo(TypeId::create<Vector>());
    PrintTypeInfo(TypeId::create<Quaternion>());
    PrintTypeInfo(TypeId::create<Transform>());
    PrintTypeInfo(TypeId::create<GameObject>());
}
