#pragma clang diagnostic push
#pragma ide diagnostic ignored "misc-no-recursion"
#include "./builtins.h"

#include <ranges>
#include <valarray>

#include "./eval_env.h"
std::unordered_map<std::string, ValuePtr> getBuiltins() {
    return {
        {"apply", std::make_shared<BuiltinProcValue>(apply)},
        {"eval", std::make_shared<BuiltinProcValue>(eval)},
        {"print", std::make_shared<BuiltinProcValue>(print)},
        {"newline", std::make_shared<BuiltinProcValue>(newline)},
        {"display", std::make_shared<BuiltinProcValue>(display)},
        {"displayln", std::make_shared<BuiltinProcValue>(display_ln)},
        {"error", std::make_shared<BuiltinProcValue>(error)},
        {"exit", std::make_shared<BuiltinProcValue>(exit_)},
        {"atom?", std::make_shared<BuiltinProcValue>(is_atom)},
        {"boolean?", std::make_shared<BuiltinProcValue>(is_boolean)},
        {"number?", std::make_shared<BuiltinProcValue>(is_number)},
        {"integer?", std::make_shared<BuiltinProcValue>(is_integer)},
        {"list?", std::make_shared<BuiltinProcValue>(is_list)},
        {"null?", std::make_shared<BuiltinProcValue>(is_null)},
        {"pair?", std::make_shared<BuiltinProcValue>(is_pair)},
        {"procedure?", std::make_shared<BuiltinProcValue>(is_procedure)},
        {"string?", std::make_shared<BuiltinProcValue>(is_string)},
        {"symbol?", std::make_shared<BuiltinProcValue>(is_symbol)},
        {"+", std::make_shared<BuiltinProcValue>(add)},
        {"-", std::make_shared<BuiltinProcValue>(subtract)},
        {"*", std::make_shared<BuiltinProcValue>(multiply)},
        {"/", std::make_shared<BuiltinProcValue>(divide)},
        {"abs", std::make_shared<BuiltinProcValue>(abs_)},
        {"expt", std::make_shared<BuiltinProcValue>(expt)},
        {"quotient", std::make_shared<BuiltinProcValue>(quotient)},
        {"remainder", std::make_shared<BuiltinProcValue>(remainder_)},
        {"modulo", std::make_shared<BuiltinProcValue>(modulo)},
        {"eq?", std::make_shared<BuiltinProcValue>(eq)},
        {"equal?", std::make_shared<BuiltinProcValue>(equal)},
        {"eq_sym?", std::make_shared<BuiltinProcValue>(eq)},
        {"not", std::make_shared<BuiltinProcValue>(not_)},
        {"=", std::make_shared<BuiltinProcValue>(eq)},
        {"<", std::make_shared<BuiltinProcValue>(lt)},
        {">", std::make_shared<BuiltinProcValue>(gt)},
        {"<=", std::make_shared<BuiltinProcValue>(lte)},
        {">=", std::make_shared<BuiltinProcValue>(gte)},
        {"even?", std::make_shared<BuiltinProcValue>(even)},
        {"odd?", std::make_shared<BuiltinProcValue>(odd)},
        {"zero?", std::make_shared<BuiltinProcValue>(zero)},
        {"cons", std::make_shared<BuiltinProcValue>(cons)},
        {"car", std::make_shared<BuiltinProcValue>(car)},
        {"cdr", std::make_shared<BuiltinProcValue>(cdr)},
        {"list", std::make_shared<BuiltinProcValue>(list)},
        {"length", std::make_shared<BuiltinProcValue>(length)},
        {"append", std::make_shared<BuiltinProcValue>(append)},
        {"map", std::make_shared<BuiltinProcValue>(map)},
        {"filter", std::make_shared<BuiltinProcValue>(filter)},
        {"reduce", std::make_shared<BuiltinProcValue>(reduce)},
    };
}

ValuePtr apply(BuiltinParams params) {
    Checker::checkParams(params, 2, 2);
    const auto& proc = params[0];
    auto args = params[1];
    if (proc->getType() == ValueType::LambdaValue) {
        return proc->as<LambdaValue>()->apply(args->toVector());
    }
    if (proc->getType() == ValueType::BuiltinProcValue) {
        return proc->as<BuiltinProcValue>()->apply(args->toVector());
    }
    std::vector<ValuePtr> arg_list;
    while (args->getType() == ValueType::PairValue) {
        arg_list.push_back(args->as<PairValue>()->getCar());
        args = args->as<PairValue>()->getCdr();
    }
    return {};
}

ValuePtr eval(BuiltinParams params) {
    Checker::checkParams(params, 1, 1);
    return GLOBAL_ENV->eval(params[0]);
}

ValuePtr print(BuiltinParams params) {
    for (const auto& i : params) {
        std::cout << i->toString() << std::endl;
    }
    return std::make_shared<NilValue>();
}

ValuePtr newline(BuiltinParams params) {
    Checker::checkParams(params, 0, 0);
    std::cout << std::endl;
    return std::make_shared<NilValue>();
}

ValuePtr display(BuiltinParams params) {
    for (const auto& i : params) {
        if (i->getType() == ValueType::StringValue) {
            std::cout << i->as<StringValue>()->getValue();
            continue;
        }
        std::cout << i->toString();
    }
    return std::make_shared<NilValue>();
}

ValuePtr display_ln(BuiltinParams params) {
    display(params);
    return newline({});
}

ValuePtr error(BuiltinParams params) {
    Checker::checkParams(params, 0, 1);
    if (params.empty()) {
        throw LispError({});
    }
    throw LispError(params[0]->toString());
}

ValuePtr exit_(BuiltinParams params) {
    Checker::checkParams(params, 0, 1);
    if (params.size() == 1) {
        Checker::checkNumber(params[0]);
        exit(static_cast<int>(params[0]->asNumber()));
    }
    exit(0);
}

ValuePtr is_atom(BuiltinParams params) {
    Checker::checkParams(params, 1, 1);
    return std::make_shared<BooleanValue>(Checker::checkTypeInList(
        {ValueType::BooleanValue, ValueType::NumericValue,
         ValueType::SymbolValue, ValueType::StringValue, ValueType::NilValue},
        params[0]->getType()));
}

ValuePtr is_boolean(BuiltinParams params) {
    Checker::checkParams(params, 1, 1);
    return std::make_shared<BooleanValue>(params[0]->getType() ==
                                          ValueType::BooleanValue);
}

ValuePtr is_number(BuiltinParams params) {
    Checker::checkParams(params, 1, 1);
    return std::make_shared<BooleanValue>(params[0]->getType() ==
                                          ValueType::NumericValue);
}

ValuePtr is_integer(BuiltinParams params) {
    Checker::checkParams(params, 1, 1);
    return std::make_shared<BooleanValue>(
        (params[0]->getType() == ValueType::NumericValue) &&
        (params[0]->asNumber() == static_cast<int>(params[0]->asNumber())));
}

ValuePtr is_list(BuiltinParams params) {
    Checker::checkParams(params, 1, 1);
    if (params[0]->getType() == ValueType::NilValue) {
        return std::make_shared<BooleanValue>(true);
    }
    if (params[0]->getType() != ValueType::PairValue) {
        return std::make_shared<BooleanValue>(false);
    }
    auto tmp = params[0]->as<PairValue>()->getCdr();
    while (tmp->getType() == ValueType::PairValue) {
        tmp = tmp->as<PairValue>()->getCdr();
    }
    if (tmp->getType() == ValueType::NilValue) {
        return std::make_shared<BooleanValue>(true);
    }
    return std::make_shared<BooleanValue>(false);
}

ValuePtr is_null(BuiltinParams params) {
    Checker::checkParams(params, 1, 1);
    return std::make_shared<BooleanValue>(params[0]->isNil());
}

ValuePtr is_pair(BuiltinParams params) {
    Checker::checkParams(params, 1, 1);
    return std::make_shared<BooleanValue>(params[0]->getType() ==
                                          ValueType::PairValue);
}

ValuePtr is_procedure(BuiltinParams params) {
    Checker::checkParams(params, 1, 1);
    return std::make_shared<BooleanValue>(Checker::checkTypeInList(
        {ValueType::BuiltinProcValue, ValueType::LambdaValue},
        params[0]->getType()));
}

ValuePtr is_string(BuiltinParams params) {
    Checker::checkParams(params, 1, 1);
    return std::make_shared<BooleanValue>(params[0]->getType() ==
                                          ValueType::StringValue);
}

ValuePtr is_symbol(BuiltinParams params) {
    Checker::checkParams(params, 1, 1);
    return std::make_shared<BooleanValue>(params[0]->getType() ==
                                          ValueType::SymbolValue);
}

ValuePtr add(BuiltinParams params) {
    double result = 0;
    for (const auto& i : params) {
        Checker::checkNumber(i);
        result += i->asNumber();
    }
    return std::make_shared<NumericValue>(result);
}

ValuePtr subtract(BuiltinParams params) {
    Checker::checkParams(params, 1, 2);
    Checker::checkNumeric(params);
    if (params.size() == 1) {
        return std::make_shared<NumericValue>(-params[0]->asNumber());
    }
    return std::make_shared<NumericValue>(params[0]->asNumber() -
                                          params[1]->asNumber());
}

ValuePtr multiply(BuiltinParams params) {
    double result = 1;
    Checker::checkNumeric(params);
    for (const auto& i : params) {
        result *= i->asNumber();
    }
    return std::make_shared<NumericValue>(result);
}

ValuePtr divide(BuiltinParams params) {
    Checker::checkParams(params, 1, 2);
    Checker::checkNumeric(params);
    if (params.size() == 1) {
        if (params[0]->asNumber() == 0) {
            throw LispError("Division by zero.");
        }
        return std::make_shared<NumericValue>(1 / params[0]->asNumber());
    }
    if (params[1]->asNumber() == 0) {
        throw LispError("Division by zero.");
    }
    return std::make_shared<NumericValue>(params[0]->asNumber() /
                                          params[1]->asNumber());
}

ValuePtr abs_(BuiltinParams params) {
    Checker::checkParams(params, 1, 1);
    Checker::checkNumber(params[0]);
    return std::make_shared<NumericValue>(std::abs(params[0]->asNumber()));
}

ValuePtr expt(BuiltinParams params) {
    Checker::checkParams(params, 2, 2);
    Checker::checkNumeric(params);
    return std::make_shared<NumericValue>(
        std::pow(params[0]->asNumber(), params[1]->asNumber()));
}

ValuePtr quotient(BuiltinParams params) {
    Checker::checkParams(params, 2, 2);
    Checker::checkNumeric(params);
    return std::make_shared<NumericValue>(
        static_cast<int>(params[0]->asNumber() / params[1]->asNumber()));
}

ValuePtr remainder_(BuiltinParams params) {
    Checker::checkParams(params, 2, 2);
    Checker::checkNumeric(params);
    return std::make_shared<NumericValue>(
        static_cast<int>(params[0]->asNumber()) %
        static_cast<int>(params[1]->asNumber()));
}

ValuePtr modulo(BuiltinParams params) {
    Checker::checkParams(params, 2, 2);
    Checker::checkNumeric(params);
    if (params[1]->asNumber() == 0) {
        throw LispError("Division by zero.");
    }
    auto naive_result = std::make_shared<NumericValue>(
        std::fmod(params[0]->asNumber(), params[1]->asNumber()));
    if (params[0]->asNumber() * params[1]->asNumber() < 0) {
        return modulo({multiply({std::make_shared<NumericValue>(-1),
                                 add({params[0], naive_result})}),
                       params[1]});
    }
    return naive_result;
}

ValuePtr eq(BuiltinParams params) {
    Checker::checkParams(params, 2, 2);
    if (params[0]->getType() != params[1]->getType()) {
        return std::make_shared<BooleanValue>(false);
    }
    if (Checker::checkTypeInList(
            {ValueType::BooleanValue, ValueType::NumericValue,
             ValueType::SymbolValue, ValueType::NilValue,
             ValueType::BuiltinProcValue, ValueType::LambdaValue},
            params[0]->getType())) {
        return equal(params);
    }
    return std::make_shared<BooleanValue>(params[0] == params[1]);
}

ValuePtr equal(BuiltinParams params) {
    Checker::checkParams(params, 2, 2);
    return std::make_shared<BooleanValue>(params[0]->valueEqual(*params[1]));
}

ValuePtr not_(BuiltinParams params) {
    Checker::checkParams(params, 1, 1);
    return std::make_shared<BooleanValue>(
        params[0]->valueEqual(BooleanValue(false)));
}

ValuePtr lt(BuiltinParams params) {
    Checker::checkParams(params, 2, 2);
    Checker::checkNumeric(params);
    return std::make_shared<BooleanValue>(params[0]->asNumber() <
                                          params[1]->asNumber());
}

ValuePtr gt(BuiltinParams params) {
    Checker::checkParams(params, 2, 2);
    Checker::checkNumeric(params);
    return std::make_shared<BooleanValue>(params[0]->asNumber() >
                                          params[1]->asNumber());
}

ValuePtr lte(BuiltinParams params) {
    Checker::checkParams(params, 2, 2);
    Checker::checkNumeric(params);
    return std::make_shared<BooleanValue>(params[0]->asNumber() <=
                                          params[1]->asNumber());
}

ValuePtr gte(BuiltinParams params) {
    Checker::checkParams(params, 2, 2);
    Checker::checkNumeric(params);
    return std::make_shared<BooleanValue>(params[0]->asNumber() >=
                                          params[1]->asNumber());
}

ValuePtr even(BuiltinParams params) {
    Checker::checkParams(params, 1, 1);
    Checker::checkNumber(params[0]);
    return std::make_shared<BooleanValue>(
        static_cast<int>(params[0]->asNumber()) % 2 == 0);
}

ValuePtr odd(BuiltinParams params) {
    Checker::checkParams(params, 1, 1);
    Checker::checkNumber(params[0]);
    return std::make_shared<BooleanValue>(
        static_cast<int>(params[0]->asNumber()) % 2 != 0);
}

ValuePtr zero(BuiltinParams params) {
    Checker::checkParams(params, 1, 1);
    Checker::checkNumber(params[0]);
    return std::make_shared<BooleanValue>(params[0]->asNumber() == 0);
}

ValuePtr cons(BuiltinParams params) {
    Checker::checkParams(params, 2, 2);
    return std::make_shared<PairValue>(params[0], params[1]);
}

ValuePtr car(BuiltinParams params) {
    Checker::checkParams(params, 1, 1);
    Checker::checkPairs(params);
    return params[0]->as<PairValue>()->getCar();
}

ValuePtr cdr(BuiltinParams params) {
    Checker::checkParams(params, 1, 1);
    Checker::checkPairs(params);
    auto cdr_v = params[0]->as<PairValue>()->getCdr();
    if (cdr_v->getType() == ValueType::NilValue) {
        return std::make_shared<NilValue>();
    }
    return params[0]->as<PairValue>()->getCdr();
}

ValuePtr list(BuiltinParams params) {
    ValuePtr result = std::make_shared<NilValue>();
    for (const auto& param : std::ranges::reverse_view(params)) {
        result = std::make_shared<PairValue>(param, result);
    }
    return result;
}

ValuePtr length(BuiltinParams params) {
    Checker::checkParams(params, 1, 1);
    int result = 0;
    if (params[0]->getType() == ValueType::NilValue) {
        return std::make_shared<NumericValue>(result);
    }
    Checker::checkPairs(params);
    ValuePtr current = params[0];
    while (current->getType() == ValueType::PairValue) {
        result++;
        current = current->as<PairValue>()->getCdr();
    }
    if (current->getType() != ValueType::NilValue) {
        throw LispError("Length requires a proper list.");
    }
    return std::make_shared<NumericValue>(result);
}

ValuePtr append(BuiltinParams params) {
    ValuePtr result = std::make_shared<NilValue>();
    for (const auto& param : params) {
        if (param->getType() != ValueType::PairValue &&
            param->getType() != ValueType::NilValue) {
            throw LispError("Append requires a list.");
        }
        ValuePtr current = param;
        while (current->getType() == ValueType::PairValue) {
            result = std::make_shared<PairValue>(
                current->as<PairValue>()->getCar(), result);
            current = current->as<PairValue>()->getCdr();
        }
        if (current->getType() != ValueType::NilValue) {
            throw LispError("Append requires a proper list.");
        }
    }
    // reverse the result
    ValuePtr current = result;
    ValuePtr next = std::make_shared<NilValue>();
    ValuePtr prev = std::make_shared<NilValue>();
    while (current->getType() == ValueType::PairValue) {
        next = current->as<PairValue>()->getCdr();
        current->as<PairValue>()->setCdr(prev);
        prev = current;
        current = next;
    }
    result = prev;
    return result;
}

ValuePtr map(BuiltinParams params) {
    Checker::checkParams(params, 2, 2);
    auto result = std::vector<ValuePtr>();
    auto args = params[1]->toVector();
    if (params[0]->getType() == ValueType::LambdaValue) {
        for (auto& arg : args) {
            result.push_back(params[0]->as<LambdaValue>()->apply({arg}));
        }
        if (result.empty()) {
            return std::make_shared<NilValue>();
        }
        return std::make_shared<PairValue>(result);
    } else if (params[0]->getType() == ValueType::BuiltinProcValue) {
        for (auto& arg : args) {
            result.push_back(params[0]->as<BuiltinProcValue>()->apply({arg}));
        }
        if (result.empty()) {
            return std::make_shared<NilValue>();
        }
        return std::make_shared<PairValue>(result);
    } else {
        throw LispError("Map requires a procedure.");
    }
}

ValuePtr filter(BuiltinParams params) {
    Checker::checkParams(params, 2, 2);
    auto result = std::vector<ValuePtr>();
    auto args = params[1]->toVector();
    if (params[0]->getType() == ValueType::LambdaValue) {
        for (auto& arg : args) {
            auto res = params[0]->as<LambdaValue>()->apply({arg});
            if (res->getType() !=
                    ValueType::BooleanValue ||
                res->as<BooleanValue>()->asBool()) {
                result.push_back(arg);
            }
        }
        if (result.empty()) {
            return std::make_shared<NilValue>();
        }
        return std::make_shared<PairValue>(result);
    } else if (params[0]->getType() == ValueType::BuiltinProcValue) {
        for (auto& arg : args) {
            auto res = params[0]->as<BuiltinProcValue>()->apply({arg});
            if (res->getType() !=
                    ValueType::BooleanValue ||
                res->as<BooleanValue>()->asBool()) {
                result.push_back(arg);
            }
        }
        if (result.empty()) {
            return std::make_shared<NilValue>();
        }
        return std::make_shared<PairValue>(result);
    } else {
        throw LispError("Filter requires a procedure.");
    }
}

ValuePtr reduce(BuiltinParams params) {
    Checker::checkParams(params, 2, 2);
    if (params[1]->getType() != ValueType::PairValue) {
        throw LispError("Reduce requires a list.");
    }
    if (length({params[1]})->asNumber() == 1) {
        return car({params[1]});
    }
    if (params[0]->getType() == ValueType::LambdaValue) {
        auto arg1 = car({params[1]});
        auto arg2 = reduce({params[0], cdr({params[1]})});
        return params[0]->as<LambdaValue>()->apply({arg1, arg2});
    } else if (params[0]->getType() == ValueType::BuiltinProcValue) {
        auto arg1 = car({params[1]});
        auto arg2 = reduce({params[0], cdr({params[1]})});
        return params[0]->as<BuiltinProcValue>()->apply({arg1, arg2});
    } else {
        throw LispError("Reduce requires a procedure.");
    }
}
#pragma clang diagnostic pop