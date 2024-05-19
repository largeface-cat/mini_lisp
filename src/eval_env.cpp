#include "eval_env.h"

#include <utility>
using namespace std::literals;
ValuePtr EvalEnv::eval(ValuePtr expr) {
    std::vector<ValuePtr> v = expr->toVector();
    if (v[0]->asSymbol() == "define"s) {
        if (auto name = v[1]->asSymbol()) {
            define(*name, eval(v[2]));
            return std::make_shared<NilValue>();
        } else {
            throw LispError("Malformed define.");
        }
    }
    if (auto name = expr->asSymbol()) {
        auto value = env.find(*name);
        if (value != env.end()) {
            return value->second;
        } else {
            throw LispError("Variable " + *name + " not defined.");
        }
    }
    if (expr->isSelfEvaluating()) {
        return expr;
    } else if (expr->isNil()) {
        throw LispError("Evaluating nil is prohibited.");
    } else {
        throw LispError("Unimplemented");
    }
}

void EvalEnv::define(const std::string& symbol, ValuePtr value) {
    env[symbol] = std::move(value);
}