//
// Created by 猫猫 on 2024/5/5.
//

#include "value.h"


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