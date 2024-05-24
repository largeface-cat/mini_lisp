#include "./eval_env.h"

//#include <utility>

#include "./forms.h"
std::shared_ptr<EvalEnv> EvalEnv::createGlobal() {
    struct make_shared_enabler : public EvalEnv {};
    auto _env = std::make_shared<make_shared_enabler>();
    _env->setEnv(getBuiltins());
    return _env;
}

std::shared_ptr<EvalEnv> EvalEnv::createNew(
    const std::vector<std::string>& params, const std::vector<ValuePtr>& args) {
    struct make_shared_enabler : public EvalEnv {};
    auto _env = std::make_shared<make_shared_enabler>();
    std::unordered_map<std::string, ValuePtr> env_map;
    for (size_t i = 0; i < params.size(); i++) {
        env_map[params[i]] = args[i];
    }
    _env->setEnv(env_map);
    return _env;
}

void EvalEnv::setParent(std::shared_ptr<EvalEnv>& p) {
    this->parent = p;
}

void EvalEnv::setEnv(const std::unordered_map<std::string, ValuePtr>& env_map) {
    env = env_map;
}

ValuePtr EvalEnv::eval(ValuePtr expr) {
    std::vector<ValuePtr> v = expr->toVector();
    if (expr->getType() == ValueType::PairValue) {
        auto* pair = expr->as<PairValue>();
        if (auto name = pair->getCar()->asSymbol()) {
            if (SPECIAL_FORMS.find(*name) != SPECIAL_FORMS.end()) {
                return SPECIAL_FORMS.at(*name)(
                    expr->as<PairValue>()->getCdr()->toVector(), *this);
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
        return lookupBinding(*name);
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
    } else if (typeid(*proc) == typeid(LambdaValue)) {
        return proc->as<LambdaValue>()->apply(args);
    } else {
        throw LispError("Unimplemented");
    }
}

void EvalEnv::define(const std::string& symbol, ValuePtr value) {
    env[symbol] = std::move(value);
}

ValuePtr EvalEnv::lookupBinding(const std::string& symbol) {
    auto it = env.find(symbol);
    if (it != env.end()) {
        return it->second;
    }
    if (parent) {
        return parent->lookupBinding(symbol);
    }
    throw LispError("Variable " + symbol + " not defined.");
}