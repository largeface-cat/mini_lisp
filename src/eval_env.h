#ifndef EVAL_ENV_H
#define EVAL_ENV_H
#include <unordered_map>
#include "./value.h"
#include "./error.h"
class EvalEnv {
private:
    std::unordered_map<std::string, ValuePtr> env{};
    static ValuePtr evalSelfEvaluating(ValuePtr expr);
    static ValuePtr evalSymbol(ValuePtr expr);
    static ValuePtr evalPair(ValuePtr expr);
public:
    EvalEnv() = default;
    ValuePtr eval(ValuePtr expr);
    void define(const std::string& symbol, ValuePtr value);
};
#endif  // EVAL_ENV_H
