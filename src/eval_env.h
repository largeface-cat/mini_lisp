#ifndef EVAL_ENV_H
#define EVAL_ENV_H
#include <unordered_map>
#include <algorithm>
#include <iterator>
#include "./value.h"
#include "./error.h"
#include "./builtins.h"
class EvalEnv {
private:
    std::unordered_map<std::string, ValuePtr> env{};
    static ValuePtr evalSelfEvaluating(ValuePtr expr);
    static ValuePtr evalSymbol(ValuePtr expr);
    static ValuePtr evalPair(ValuePtr expr);
public:
    EvalEnv() {
        env = getBuiltins();
    };
    ValuePtr eval(ValuePtr expr);
    std::vector<ValuePtr> evalList(const std::vector<ValuePtr>& list);
    ValuePtr apply(const ValuePtr& proc, const std::vector<ValuePtr>& args);
    void define(const std::string& symbol, ValuePtr value);
};
#endif  // EVAL_ENV_H
