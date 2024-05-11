#ifndef PARSER_H
#define PARSER_H
#include <deque>

#include "tokenizer.h"
#include "value.h"
class Parser {
private:
    std::deque<TokenPtr> tokens;

public:
    Parser(std::deque<TokenPtr> tokens): tokens{std::move(tokens)} {};
    ValuePtr parse();
    ValuePtr parseTails();
};
#endif  // PARSER_H
