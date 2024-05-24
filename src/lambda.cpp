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
};
