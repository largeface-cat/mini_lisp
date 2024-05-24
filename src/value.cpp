#include "./value.h"


std::string Value::toString() const {
    return "(UNKNOWN)";
}

std::string StringValue::toString() const {
    return "\""+value+"\"";
}

std::string BooleanValue::toString() const {
    return value ? "#t" : "#f";
}

std::string NumericValue::toString() const {
    if (value == (int)value) {
        return std::to_string((int)value);
    }
    return std::to_string(value);
}

std::string NilValue::toString() const {
    return "()";
}

std::string SymbolValue::toString() const {
    return value;
}

std::string PairValue::toString() const {
    std::string result = "(";
    result += car->toString();
    ValuePtr current = cdr;
    while (current->getType() == ValueType::PairValue) {
        result += " ";
        result += current->as<PairValue>()->car->toString();
        current = current->as<PairValue>()->cdr;
    }
    if (current->getType() != ValueType::NilValue) {
        result += " . ";
        result += current->toString();
    }
    result += ")";
    return result;
}

std::string BuiltinProcValue::toString() const {
    return "#<procedure>";
}



std::vector<ValuePtr> Value::toVector() const {
    throw std::runtime_error("Not a proper list");
}

std::vector<ValuePtr> StringValue::toVector() const {
    return {std::make_shared<StringValue>(value)};
}

std::vector<ValuePtr> BooleanValue::toVector() const {
    return {std::make_shared<BooleanValue>(value)};
}

std::vector<ValuePtr> NumericValue::toVector() const {
    return {std::make_shared<NumericValue>(value)};
}

std::vector<ValuePtr> NilValue::toVector() const {
    return {};
}

std::vector<ValuePtr> SymbolValue::toVector() const {
    return {std::make_shared<SymbolValue>(value)};
}


std::vector<ValuePtr> PairValue::toVector() const {
    std::vector<ValuePtr> result;
    result.push_back(car);
    ValuePtr current = cdr;
    while (current->getType() == ValueType::PairValue) {
        result.push_back(current->as<PairValue>()->car);
        current = current->as<PairValue>()->cdr;
    }
    if (current->getType() != ValueType::NilValue) {
        throw std::runtime_error("Not a proper list");
    }
    return result;
}

std::optional<std::string> Value::asSymbol() const {
    return std::nullopt;
}

std::optional<std::string> SymbolValue::asSymbol() const {
    return value;
}

ValuePtr BuiltinProcValue::apply(const std::vector<ValuePtr>& args) {
    return func(args);
}

