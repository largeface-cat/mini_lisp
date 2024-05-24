#include "./builtins.h"
#include "./lambda.h"

#include <ranges>
#include <valarray>
std::unordered_map<std::string, ValuePtr> getBuiltins() {
    return {
        {"apply", std::make_shared<BuiltinProcValue>(apply)},
        {"print", std::make_shared<BuiltinProcValue>(print)},
        {"newline", std::make_shared<BuiltinProcValue>(newline)},
        {"display", std::make_shared<BuiltinProcValue>(display)},
        {"exit", std::make_shared<BuiltinProcValue>(exit_)},
        {"+", std::make_shared<BuiltinProcValue>(add)},
        {"-", std::make_shared<BuiltinProcValue>(subtract)},
        {"*", std::make_shared<BuiltinProcValue>(multiply)},
        {"/", std::make_shared<BuiltinProcValue>(divide)},
        {"abs", std::make_shared<BuiltinProcValue>(abs_)},
        {"expt", std::make_shared<BuiltinProcValue>(expt)},
        {"quotient", std::make_shared<BuiltinProcValue>(quotient)},
        {"remainder", std::make_shared<BuiltinProcValue>(remainder_)},
        {"modulo", std::make_shared<BuiltinProcValue>(modulo)},
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
    };
}

ValuePtr apply(BuiltinParams params) {
    Checker::checkParams(params, 2, 2);
    auto proc = params[0];
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
        std::cout << i->toString();
    }
    return std::make_shared<NilValue>();
}

ValuePtr exit_(BuiltinParams params) {
    Checker::checkParams(params, 0, 1);
    if (params.size() == 1) {
        Checker::checkNumber(params[0]);
        exit(static_cast<int>(params[0]->asNumber()));
    }
    exit(0);
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
    return std::make_shared<NumericValue>(
        std::fmod(params[0]->asNumber(), params[1]->asNumber()));
}

ValuePtr eq(BuiltinParams params) {
    Checker::checkParams(params, 2, 2);
    return std::make_shared<BooleanValue>(params[0] == params[1]);
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
    Checker::checkPairs(params);
    int result = 0;
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
        if (param->getType() != ValueType::PairValue) {
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