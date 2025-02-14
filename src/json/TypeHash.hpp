//
// Created by Riebers on 11.01.2025.
//

#pragma once

#include <cstdint>
namespace Reflection {
    // https://stackoverflow.com/questions/35941045/can-i-obtain-c-type-names-in-a-constexpr-way
    // asnwer by einpoklum

    template<typename T>
    constexpr std::string_view TypeName();

    template<>
    constexpr std::string_view TypeName<void>() {
        return "void";
    }

    namespace detail {
        using type_name_prober = void;

        template<typename T>
        constexpr std::string_view wrapped_type_name() {
#ifdef __clang__
            return __PRETTY_FUNCTION__;
#elif defined(__GNUC__)
            return __PRETTY_FUNCTION__;
#elif defined(_MSC_VER)
            return __FUNCSIG__;
#else
#error "Unsupported compiler"
#endif
        }

        constexpr std::size_t wrapped_type_name_prefix_length() {
            return wrapped_type_name<type_name_prober>().find(TypeName<type_name_prober>());
        }

        constexpr std::size_t wrapped_type_name_suffix_length() {
            return wrapped_type_name<type_name_prober>().length() - wrapped_type_name_prefix_length() -
                   TypeName<type_name_prober>().length();
        }
    } // namespace detail

#pragma warning(disable : 4702)
    template<typename T>
    constexpr std::string_view TypeName() {
        constexpr auto wrapped_name = detail::wrapped_type_name<T>();
        constexpr auto prefix_length = detail::wrapped_type_name_prefix_length();
        constexpr auto suffix_length = detail::wrapped_type_name_suffix_length();
        constexpr auto type_name_length = wrapped_name.length() - prefix_length - suffix_length;
        if constexpr (constexpr auto offset =
                              wrapped_name.substr(prefix_length, type_name_length).find_first_of(" ");
                      offset != std::string_view::npos) {
            return wrapped_name.substr(prefix_length + offset + 1, type_name_length - offset - 1);
        }
        return wrapped_name.substr(prefix_length, type_name_length);
    }


    /**
     * HASHING
     */

    constexpr std::uint64_t hash(std::string_view str) {
        std::uint64_t hash_value = 0xcbf29ce484222325ULL;
        constexpr std::uint64_t prime = 0x100000001b3ULL;
        for (char c: str) {
            hash_value ^= static_cast<std::uint64_t>(c);
            hash_value *= prime;
        }
        return hash_value;
    }

    /**
     * TYPE ID
     */

    template<typename Type>
    constexpr std::uint64_t TypeId() {
        return hash(TypeName<Type>());
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
} // namespace Reflection
