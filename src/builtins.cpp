#include <ranges>

#include "./builtins.h"
std::unordered_map<std::string, ValuePtr> getBuiltins() {
    return {
        {"print", std::make_shared<BuiltinProcValue>(print)},
        {"+", std::make_shared<BuiltinProcValue>(add)},
        {"-", std::make_shared<BuiltinProcValue>(subtract)},
        {"*", std::make_shared<BuiltinProcValue>(multiply)},
        {"/", std::make_shared<BuiltinProcValue>(divide)},
        {"=", std::make_shared<BuiltinProcValue>(eq)},
        {"<", std::make_shared<BuiltinProcValue>(lt)},
        {">", std::make_shared<BuiltinProcValue>(gt)},
        {"<=", std::make_shared<BuiltinProcValue>(lte)},
        {">=", std::make_shared<BuiltinProcValue>(gte)},
        {"cons", std::make_shared<BuiltinProcValue>(cons)},
        {"car", std::make_shared<BuiltinProcValue>(car)},
        {"cdr", std::make_shared<BuiltinProcValue>(cdr)},
        {"list", std::make_shared<BuiltinProcValue>(list)},
        {"length", std::make_shared<BuiltinProcValue>(length)},
    };
}
ValuePtr print(const std::vector<ValuePtr>& params) {
    for (const auto& i : params) {
        std::cout << i->toString() << std::endl;
    }
    return std::make_shared<NilValue>();
}
ValuePtr add(const std::vector<ValuePtr>& params) {
    double result = 0;
    for (const auto& i : params) {
        if (!i->isNumber()) {
            throw LispError("Cannot add a non-numeric value.");
        }
        result += i->asNumber();
    }
    return std::make_shared<NumericValue>(result);
}

ValuePtr subtract(const std::vector<ValuePtr>& params) {
    if (params.size() == 1) {
        return std::make_shared<NumericValue>(-params[0]->asNumber());
    }
    if (params.size() != 2) {
        throw LispError("Subtract requires exactly 1 or 2 arguments.");
    }
    if (!params[0]->isNumber() || !params[1]->isNumber()) {
        throw LispError("Cannot subtract a non-numeric value.");
    }
    return std::make_shared<NumericValue>(params[0]->asNumber() - params[1]->asNumber());
}

ValuePtr multiply(const std::vector<ValuePtr>& params) {
    double result = 1;
    for (const auto& i : params) {
        if (!i->isNumber()) {
            throw LispError("Cannot multiply a non-numeric value.");
        }
        result *= i->asNumber();
    }
    return std::make_shared<NumericValue>(result);
}

ValuePtr divide(const std::vector<ValuePtr>& params) {
    if (params.size() == 1) {
        return std::make_shared<NumericValue>(1 / params[0]->asNumber());
    }
    if (params.size() != 2) {
        throw LispError("Divide requires exactly 1 or 2 arguments.");
    }
    if (!params[0]->isNumber() || !params[1]->isNumber()) {
        throw LispError("Cannot divide a non-numeric value.");
    }
    return std::make_shared<NumericValue>(params[0]->asNumber() / params[1]->asNumber());
}

ValuePtr eq(const std::vector<ValuePtr>& params) {
    if (params.size() != 2) {
        throw LispError("Eq requires exactly 2 arguments.");
    }
    return std::make_shared<BooleanValue>(params[0] == params[1]);
}

ValuePtr lt(const std::vector<ValuePtr>& params) {
    if (params.size() != 2) {
        throw LispError("Lt requires exactly 2 arguments.");
    }
    if (!params[0]->isNumber() || !params[1]->isNumber()) {
        throw LispError("Cannot compare a non-numeric value.");
    }
    return std::make_shared<BooleanValue>(params[0]->asNumber() < params[1]->asNumber());
}

ValuePtr gt(const std::vector<ValuePtr>& params) {
    if (params.size() != 2) {
        throw LispError("Gt requires exactly 2 arguments.");
    }
    if (!params[0]->isNumber() || !params[1]->isNumber()) {
        throw LispError("Cannot compare a non-numeric value.");
    }
    return std::make_shared<BooleanValue>(params[0]->asNumber() > params[1]->asNumber());
}

ValuePtr lte(const std::vector<ValuePtr>& params) {
    if (params.size() != 2) {
        throw LispError("Lte requires exactly 2 arguments.");
    }
    if (!params[0]->isNumber() || !params[1]->isNumber()) {
        throw LispError("Cannot compare a non-numeric value.");
    }
    return std::make_shared<BooleanValue>(params[0]->asNumber() <= params[1]->asNumber());
}

ValuePtr gte(const std::vector<ValuePtr>& params) {
    if (params.size() != 2) {
        throw LispError("Gte requires exactly 2 arguments.");
    }
    if (!params[0]->isNumber() || !params[1]->isNumber()) {
        throw LispError("Cannot compare a non-numeric value.");
    }
    return std::make_shared<BooleanValue>(params[0]->asNumber() >= params[1]->asNumber());
}

ValuePtr cons(const std::vector<ValuePtr>& params) {
    if (params.size() != 2) {
        throw LispError("Cons requires exactly 2 arguments.");
    }
    return std::make_shared<PairValue>(params[0], params[1]);
}

ValuePtr car(const std::vector<ValuePtr>& params) {
    if (params.size() != 1) {
        throw LispError("Car requires exactly 1 argument.");
    }
    if (params[0]->getType() != ValueType::PairValue) {
        throw LispError("Car requires a pair.");
    }
    return params[0]->as<PairValue>()->getCar();
}

ValuePtr cdr(const std::vector<ValuePtr>& params) {
    if (params.size() != 1) {
        throw LispError("Cdr requires exactly 1 argument.");
    }
    if (params[0]->getType() != ValueType::PairValue) {
        throw LispError("Cdr requires a pair.");
    }
    return params[0]->as<PairValue>()->getCdr();
}

ValuePtr list(const std::vector<ValuePtr>& params) {
    ValuePtr result = std::make_shared<NilValue>();
    for (const auto & param : std::ranges::reverse_view(params)) {
        result = std::make_shared<PairValue>(param, result);
    }
    return result;
}

ValuePtr length(const std::vector<ValuePtr>& params) {
    if (params.size() != 1) {
        throw LispError("Length requires exactly 1 argument.");
    }
    if (params[0]->getType() != ValueType::PairValue) {
        throw LispError("Length requires a pair.");
    }
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