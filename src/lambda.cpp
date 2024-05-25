#include "lambda.h"

std::string LambdaValue::toString() const {
    return "#<procedure>";
}

ValuePtr LambdaValue::apply(const std::vector<ValuePtr>& args) {
    if (args.size() != params.size()) {
        throw LispError("Invalid number of arguments.");
    }
    auto _env = EvalEnv::createNew(params, args);
    _env->setParent(parent_env);
    ValuePtr result = nullptr;
    for (const auto& expr : body) {
        result = _env->eval(expr);
    }
    return result;
}

bool LambdaValue::valueEqual(const Value& other) const {
    if (other.getType() != ValueType::LambdaValue) {
        return false;
    }
    const auto& other_lambda = dynamic_cast<const LambdaValue&>(other);
    bool flag = true;
    for (size_t i = 0; i < params.size() && flag; i++) {
        flag = params[i] == other_lambda.params[i];
    }
    for (size_t i = 0; i < body.size() && flag; i++) {
        flag = body[i]->valueEqual(*other_lambda.body[i]);
    }
    return flag;
};
