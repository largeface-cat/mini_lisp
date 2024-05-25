#include "forms.h"
extern const std::unordered_map<std::string, SpecialFormType*> SPECIAL_FORMS{
    {"define", defineForm}, {"quote", quoteForm}, {"if", ifForm},
    {"and", andForm},       {"or", orForm},       {"lambda", lambdaForm},
    {"cond", condForm},     {"let", letForm},     {"begin", beginForm},
    {"quasiquote", quasiQuoteForm}, {"unquote", unquoteForm},
    {"`", quasiQuoteForm}, {",", unquoteForm}
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

ValuePtr quoteForm(const std::vector<ValuePtr>& args,
                   [[maybe_unused]] EvalEnv& env) {
    return args[0];
}

ValuePtr ifForm(const std::vector<ValuePtr>& args, EvalEnv& env) {
    if (args.size() < 2 || args.size() > 3){
        throw LispError("Malformed if.");
    }
    auto _eval = env.eval(args[0]);
    if (_eval->getType() == ValueType::BooleanValue &&
        !_eval->as<BooleanValue>()->asBool()) {
        if (args.size() == 3) {
            return env.eval(args[2]);
        } else {
            return std::make_shared<NilValue>();
        }
    }
    return env.eval(args[1]);
}

ValuePtr andForm(const std::vector<ValuePtr>& args, EvalEnv& env) {
    if (args.empty()) {
        return std::make_shared<BooleanValue>(true);
    }
    auto _eval = ValuePtr{};
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
            if (args[i]->getType() != ValueType::PairValue && args[i]->getType() != ValueType::NilValue){
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
    return std::make_shared<LambdaValue>(params, body, env.shared_from_this());
}

ValuePtr condForm(const std::vector<ValuePtr>& args, EvalEnv& env) {
    auto result = ValuePtr{};
    for (size_t i = 0; i < args.size(); i++) {
        const auto& arg = args[i];
        if (arg->getType() != ValueType::PairValue) {
            throw LispError("Malformed cond.");
        }
        auto v = arg->toVector();

        if (v[0]->getType() != ValueType::SymbolValue ||
            v[0]->asSymbol() != "else") {
            auto condition = env.eval(v[0]);
            if (condition->getType() == ValueType::BooleanValue) {
                if (condition->valueEqual(BooleanValue(false))) {
                    continue;
                }
            }
            if (v.size() < 2) {
                return condition;
            }
            for (size_t j = 1; j < v.size() - 1; j++) {
                env.eval(v[j]);
            }
            return env.eval(v[v.size() - 1]);
        } else {
            if (i != args.size() - 1) {
                throw LispError("Malformed cond.");
            }
            if (v.size() < 2) {
                throw LispError("Undefined behavior.");
            }
            for (size_t j = 1; j < v.size() - 1; j++) {
                env.eval(v[j]);
            }
            return env.eval(v[v.size() - 1]);
        }
    }
    throw LispError("Undefined behavior.");
}

ValuePtr beginForm(const std::vector<ValuePtr>& args, EvalEnv& env) {
    auto result = ValuePtr{};
    for (const auto& arg : args) {
        result = env.eval(arg);
    }
    return result;
}

ValuePtr letForm(const std::vector<ValuePtr>& args, EvalEnv& env) {
    auto lambda_params = std::vector<std::string>{};
    auto lambda_body = std::vector<ValuePtr>{};
    auto let_args = std::vector<ValuePtr>{};
    for (size_t i = 0; i < args.size(); i++) {
        if (i == 0) {
            if (args[0]->getType() != ValueType::PairValue) {
                throw LispError("Malformed let.");
            }
            auto v = args[0]->toVector();
            for (const auto& p : v) {
                if (p->getType() != ValueType::PairValue) {
                    throw LispError("Malformed let.");
                }
                auto p_v = p->toVector();
                if (p_v.size() != 2) {
                    throw LispError("Malformed let.");
                }
                if (p_v[0]->getType() != ValueType::SymbolValue) {
                    throw LispError("Malformed let.");
                }
                lambda_params.push_back(*p_v[0]->asSymbol());
                let_args.push_back(env.eval(p_v[1]));
            }
        } else {
            lambda_body.push_back(args[i]);
        }
    }
    return std::make_shared<LambdaValue>(lambda_params, lambda_body,
                                         env.shared_from_this())
        ->apply(let_args);
}

ValuePtr quasiQuoteForm(const std::vector<ValuePtr>& args,
                        [[maybe_unused]] EvalEnv& env) {
    if (args.size() != 1) {
        throw LispError("Malformed quasiquote.");
    }
    auto current = args[0];
    while (true) {
        auto left = current->as<PairValue>()->getCar()->toVector();
        if (left[0]->getType() == ValueType::SymbolValue &&
            left[0]->asSymbol() == "unquote") {
            current->as<PairValue>()->setCar(
                env.eval(left[1]));
        }
        current = current->as<PairValue>()->getCdr();
        if (current->getType() == ValueType::NilValue) {
            break;
        }
    }
    return args[0];
}

ValuePtr unquoteForm([[maybe_unused]] const std::vector<ValuePtr>& args,
                     [[maybe_unused]] EvalEnv& env) {
    throw LispError("Undefined behavior.");
}