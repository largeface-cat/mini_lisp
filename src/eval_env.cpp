#include "./eval_env.h"
#include "./forms.h"
#include <utility>
using namespace std::literals;

ValuePtr EvalEnv::eval(ValuePtr expr) {
    std::vector<ValuePtr> v = expr->toVector();
//    if (v[0]->asSymbol() == "define"s) {
//        if (auto name = v[1]->asSymbol()) {
//            define(*name, eval(v[2]));
//            return std::make_shared<NilValue>();
//        } else {
//            throw LispError("Malformed define.");
//        }
//    }
    if (expr->getType() == ValueType::PairValue) {
        auto* pair = expr->as<PairValue>();
        if (auto name = pair->getCar()->asSymbol()) {
            if (SPECIAL_FORMS.find(*name) != SPECIAL_FORMS.end()) {
                    return SPECIAL_FORMS.at(*name)(
                    expr->as<PairValue>()->getCdr()->toVector(), *this);;
            }
        }
    }
    // end of special forms
    if (expr->getType() == ValueType::PairValue) {
        ValuePtr proc = eval(v[0]);
        std::vector<ValuePtr> args =
            evalList(std::vector<ValuePtr>(v.begin() + 1, v.end()));
        return apply(proc, args);
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

std::vector<ValuePtr> EvalEnv::evalList(const std::vector<ValuePtr>& list) {
    std::vector<ValuePtr> result;
    std::ranges::transform(
        list, std::back_inserter(result),
        [this](ValuePtr v) { return this->eval(std::move(v)); });
    return result;
}

ValuePtr EvalEnv::apply(const ValuePtr& proc,
                        const std::vector<ValuePtr>& args) {
    if (typeid(*proc) == typeid(BuiltinProcValue)) {
        return proc->as<BuiltinProcValue>()->apply(args);
    } else {
        throw LispError("Unimplemented");
    }
}

void EvalEnv::define(const std::string& symbol, ValuePtr value) {
    env[symbol] = std::move(value);
}