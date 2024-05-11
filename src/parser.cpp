#include "parser.h"
ValuePtr Parser::parse() {
    auto token = std::move(tokens.front());
    tokens.pop_front();
    if (token->getType() == TokenType::NUMERIC_LITERAL) {
        auto value = static_cast<NumericLiteralToken&>(*token).getValue();
        return std::make_shared<NumericValue>(value);
    }
    if (token->getType() == TokenType::BOOLEAN_LITERAL) {
        auto value = static_cast<BooleanLiteralToken&>(*token).getValue();
        return std::make_shared<BooleanValue>(value);
    }
    if (token->getType() == TokenType::STRING_LITERAL) {
        auto value = static_cast<StringLiteralToken&>(*token).getValue();
        return std::make_shared<StringValue>(value);
    }
    if (token->getType() == TokenType::IDENTIFIER) {
        auto value = static_cast<IdentifierToken&>(*token).getName();
        return std::make_shared<SymbolValue>(value);
    }
    if (token->getType() == TokenType::LEFT_PAREN) {
        return parseTails();
    }
    if (token->getType() == TokenType::QUOTE) {
        return std::make_shared<PairValue>(
            std::make_shared<SymbolValue>("quote"),
            std::make_shared<PairValue>(parse(), std::make_shared<NilValue>())
        );
    }
    if (token->getType() == TokenType::QUASIQUOTE) {
        return std::make_shared<PairValue>(
            std::make_shared<SymbolValue>("quasiquote"),
            std::make_shared<PairValue>(parse(), std::make_shared<NilValue>())
        );
    }
    if (token->getType() == TokenType::UNQUOTE) {
        return std::make_shared<PairValue>(
            std::make_shared<SymbolValue>("unquote"),
            std::make_shared<PairValue>(parse(), std::make_shared<NilValue>())
        );
    }
    throw std::runtime_error("Unexpected token: " + token->toString());

    // [...]
}

ValuePtr Parser::parseTails() {
    if (tokens.empty()) {
        throw std::runtime_error("Unexpected EOF");
    }

    if (tokens.front()->getType() == TokenType::RIGHT_PAREN) {
        tokens.pop_front();
        return std::make_shared<NilValue>();
    }
    auto car = parse();
    if (tokens.front()->getType() == TokenType::DOT) {
        tokens.pop_front();
        auto cdr = parse();
        if (tokens.front()->getType() != TokenType::RIGHT_PAREN) {
            throw std::runtime_error("Expecting ')'");
        }
        tokens.pop_front();
        return std::make_shared<PairValue>(car, cdr);
    }
    auto cdr = parseTails();
    return std::make_shared<PairValue>(car, cdr);
}