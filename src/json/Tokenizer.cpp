//
// Created by Riebers on 17.01.2025.
//

#include "Tokenizer.hpp"


namespace json::detail {

    tl::expected<Token, JsonError> Tokenizer::parse_string() {
        std::string result;
        ++m_pos;

        while (m_pos < m_input.size()) {
            if (m_input[m_pos] == '"') {
                return Token{TokenType::String, result};
            }
            // Escape sequence
            if (m_input[m_pos] == '\\') {
                ++m_pos;
                if (m_pos < m_input.size()) {
                    if (m_input[m_pos] == 'n') {
                        result += '\n';
                    } else if (m_input[m_pos] == '"') {
                        result += '"';
                    } else if (m_input[m_pos] == 't') {
                        result += '\t';
                    } else if (m_input[m_pos] == '\\') {
                        result += '\\';
                    } else if (m_input[m_pos] == '/') {
                        result += '/';
                    } else if (m_input[m_pos] == 'f') {
                        result += '\f';
                    } else if (m_input[m_pos] == 'r') {
                        result += '\r';
                    } else if (m_input[m_pos] == 'u') {
                        return tl::unexpected{JsonError{Error::unicode, m_pos}};
                    } else {
                        return tl::unexpected{JsonError{Error::invalid_escape_sequence, m_pos}};
                    }
                } else {
                    return tl::unexpected{JsonError{Error::unquoted_string, --m_pos}};
                }
            } else {
                result += m_input[m_pos];
            }
            ++m_pos;
        }
        return tl::unexpected{JsonError{Error::unquoted_string, --m_pos}};
    }

    tl::expected<Token, JsonError> Tokenizer::parse_null_or_bool() {
        if (m_input.substr(m_pos, 5).find("null") != std::string::npos) {
            m_pos += 3;
            return Token{TokenType::Null, "null"};
        }
        if (m_input.substr(m_pos, 5).find("true") != std::string::npos) {
            m_pos += 3;
            return Token{TokenType::Boolean, "true"};
        }
        if (m_input.substr(m_pos, 5).find("false") != std::string::npos) {
            m_pos += 4;
            return Token{TokenType::Boolean, "false"};
        }
        return tl::unexpected{JsonError{Error::invalid_type, m_pos}};
    }
    tl::expected<Token, JsonError> Tokenizer::parse_number() {
        std::string result;

        while (m_pos < m_input.size()) {
            if (std::isdigit(m_input[m_pos]) || m_input[m_pos] == '.') {
                result += m_input[m_pos];
            } else {
                --m_pos;
                break;
            }
            ++m_pos;
        }
        return Token{TokenType::Number, result};
    }

    tl::expected<Tokens, JsonError> Tokenizer::tokenize(std::string_view input) {
        m_input = input;
        m_tokens.clear();

        for (m_pos = 0; m_pos < m_input.size(); ++m_pos) {
            if (m_input[m_pos] == '{') {
                m_tokens.emplace_back(TokenType::LeftBrace, "{");
            } else if (m_input[m_pos] == '}') {
                m_tokens.emplace_back(TokenType::RightBrace, "}");
            } else if (m_input[m_pos] == '[') {
                m_tokens.emplace_back(TokenType::LeftBracket, "[");
            } else if (m_input[m_pos] == ']') {
                m_tokens.emplace_back(TokenType::RightBracket, "]");
            } else if (m_input[m_pos] == ',') {
                m_tokens.emplace_back(TokenType::Comma, ",");
            } else if (m_input[m_pos] == ':') {
                m_tokens.emplace_back(TokenType::KeyValueSeparator, ":");
            } else if (m_input[m_pos] == '"') {
                if (auto const result = parse_string(); result.has_value()) {
                    m_tokens.emplace_back(*result);
                } else {
                    return tl::unexpected{result.error()};
                }
            } else if (std::isalpha(m_input[m_pos])) {
                if (auto const result = parse_null_or_bool(); result.has_value()) {
                    m_tokens.emplace_back(*result);
                } else {
                    return tl::unexpected{result.error()};
                }
            } else if (std::isdigit(m_input[m_pos])) {
                if (auto const result = parse_number(); result.has_value()) {
                    m_tokens.emplace_back(*result);
                } else {
                    return tl::unexpected{result.error()};
                }
            }
        }
        return m_tokens;
    }
} // namespace json::detail
