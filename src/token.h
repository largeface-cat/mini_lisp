#ifndef TOKEN_H
#define TOKEN_H

#include <memory>
#include <optional>
#include <ostream>
#include <string>
#include <utility>

enum class TokenType {
    LEFT_PAREN,
    RIGHT_PAREN,
    QUOTE,
    QUASIQUOTE,
    UNQUOTE,
    DOT,
    BOOLEAN_LITERAL,
    NUMERIC_LITERAL,
    STRING_LITERAL,
    IDENTIFIER,
};

class Token;
using TokenPtr = std::unique_ptr<Token>;

class Token {
private:
    TokenType type;

protected:
    explicit Token(TokenType type) : type{type} {}

public:
    virtual ~Token() = default;

    static TokenPtr fromChar(char c);
    static TokenPtr dot();

    [[nodiscard]] TokenType getType() const {
        return type;
    }
    [[nodiscard]] virtual std::string toString() const;
};

class BooleanLiteralToken : public Token {
private:
    bool value;

public:
    explicit BooleanLiteralToken(bool value) : Token(TokenType::BOOLEAN_LITERAL), value{value} {}

    static std::unique_ptr<BooleanLiteralToken> fromChar(char c);

    [[nodiscard]] bool getValue() const {
        return value;
    }
    [[nodiscard]] std::string toString() const override;
};

class NumericLiteralToken : public Token {
private:
    double value;

public:
    explicit NumericLiteralToken(double value) : Token(TokenType::NUMERIC_LITERAL), value{value} {}

    [[nodiscard]] double getValue() const {
        return value;
    }
    [[nodiscard]] std::string toString() const override;
};

class StringLiteralToken : public Token {
private:
    std::string value;

public:
    explicit StringLiteralToken(std::string value) : Token(TokenType::STRING_LITERAL), value{std::move(value)} {}

    [[nodiscard]] const std::string& getValue() const {
        return value;
    }
    [[nodiscard]] std::string toString() const override;
};

class IdentifierToken : public Token {
private:
    std::string name;

public:
    explicit IdentifierToken(std::string name) : Token(TokenType::IDENTIFIER), name{std::move(name)} {}

    [[nodiscard]] const std::string& getName() const {
        return name;
    }
    [[nodiscard]] std::string toString() const override;
};

std::ostream& operator<<(std::ostream& os, const Token& token);

#endif
