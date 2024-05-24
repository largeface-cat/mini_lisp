#include "forms.h"
extern const std::unordered_map<std::string, SpecialFormType*> SPECIAL_FORMS{
    {"define", defineForm},
    {"quote", quoteForm},
    {"if", ifForm},
    {"and", andForm},
    {"or", orForm},
    {"lambda", lambdaForm},
};

ValuePtr defineForm(const std::vector<ValuePtr>& args, EvalEnv& env) {
    if (auto name = args[0]->asSymbol()) {
        env.define(*name, env.eval(args[1]));
    } else if (args[0]->getType() == ValueType::PairValue) {
        auto v = args[0]->toVector();
        if (v.empty()) {
            throw LispError("Malformed define.");
        }
        if (v[0]->getType() != ValueType::SymbolValue) {
            throw LispError("Malformed define.");
        }
        name = v[0]->asSymbol();
        auto _args = args;
        _args[0] = _args[0]->as<PairValue>()->getCdr();
        auto lambda = lambdaForm(_args, env);
        env.define(*name, lambda);
    } else {
        throw LispError("Malformed define.");

    }
    return std::make_shared<NilValue>();
}

ValuePtr quoteForm(const std::vector<ValuePtr>& args, EvalEnv& env) {
    return args[0];
}

ValuePtr ifForm(const std::vector<ValuePtr>& args, EvalEnv& env) {
    if (args.size() != 3) {
        throw LispError("Malformed if.");
    }
    auto _eval = env.eval(args[0]);
    if (_eval->getType() == ValueType::BooleanValue &&
        !_eval->as<BooleanValue>()->asBool()) {
        return env.eval(args[2]);
    }
    return env.eval(args[1]);
}

ValuePtr andForm(const std::vector<ValuePtr>& args, EvalEnv& env) {
    if (args.empty()) {
        return std::make_shared<BooleanValue>(true);
    }
    auto _eval = ValuePtr {};
    for (const auto& arg : args) {
        _eval = env.eval(arg);
        if (_eval->getType() == ValueType::BooleanValue &&
            !_eval->as<BooleanValue>()->asBool()) {
            return std::make_shared<BooleanValue>(false);
        }
    }
    return _eval;
}

ValuePtr orForm(const std::vector<ValuePtr>& args, EvalEnv& env) {
    for (const auto& arg : args) {
        auto _eval = env.eval(arg);
        if (_eval->getType() != ValueType::BooleanValue ||
            _eval->as<BooleanValue>()->asBool()) {
            return _eval;
        }
    }
    return std::make_shared<BooleanValue>(false);
}

ValuePtr lambdaForm(const std::vector<ValuePtr>& args, EvalEnv& env) {
    std::vector<std::string> params;
    std::vector<ValuePtr> body;
    for (size_t i = 0; i < args.size(); i++) {
        if (i == 0) {
            if (args[i]->getType() != ValueType::PairValue) {
                throw LispError("Malformed lambda.");
            }
            auto v = args[i]->toVector();
            for (const auto& p : v) {
                if (auto name = p->asSymbol()) {
                    params.push_back(*name);
                } else {
                    throw LispError("Malformed lambda.");
                }
            }
        } else {
            body.push_back(args[i]);
        }
    }
    return std::make_shared<LambdaValue>(params, body);
}