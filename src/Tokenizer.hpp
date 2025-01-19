//
// Created by Riebers on 17.01.2025.
//

#pragma once

#include <filesystem>
#include <string_view>
#include <tl/expected.hpp>
namespace json {
    enum class Error {
        ok,
        unquoted_token,

        invalid_type,
        missing_comma,
        unquoted_string,
        braces_missmatch,
        invalid_escape_sequence,
        trailing_comma,
        type_mismatch,
        unicode,

        invalid_file_path,
    };

    struct JsonError {
        Error error{Error::ok};
        size_t pos{0};
    };

    enum class TokenType {
        LeftBrace,
        RightBrace,
        LeftBracket,
        RightBracket,
        Comma,
        KeyValueSeparator,
        Number,
        String,
        Null,
        Boolean,

        Unknown,
    };
    struct Token {
        TokenType type;
        std::string value;
    };

    using Tokens = std::vector<Token>;
    namespace detail {
        class Tokenizer {

        private:
            std::string_view m_input{};
            Tokens m_tokens{};
            // indicates current position in string
            std::size_t m_pos{};

            tl::expected<Token, JsonError> parse_string();
            tl::expected<Token, JsonError> parse_null_or_bool();
            tl::expected<Token, JsonError> parse_number();

        public:
            Tokenizer() = default;

            tl::expected<Tokens, JsonError> tokenize(std::string_view input);
        };


    } // namespace detail
} // namespace json
