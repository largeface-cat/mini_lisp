#include "./value.h"
#include "./eval_env.h"

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

bool Value::valueEqual(const Value& other) const {
    return this == &other;
}

bool StringValue::valueEqual(const Value& other) const {
    if (other.getType() != ValueType::StringValue) {
        return false;
    }
    auto& otherString = dynamic_cast<const StringValue&>(other);
    return value == otherString.value;
}

bool BooleanValue::valueEqual(const Value& other) const {
    if (other.getType() != ValueType::BooleanValue) {
        return false;
    }
    auto& otherBoolean = dynamic_cast<const BooleanValue&>(other);
    return value == otherBoolean.value;
}

bool NumericValue::valueEqual(const Value& other) const {
    if (other.getType() != ValueType::NumericValue) {
        return false;
    }
    auto& otherNumeric = dynamic_cast<const NumericValue&>(other);
    return value == otherNumeric.value;
}

bool NilValue::valueEqual(const Value& other) const {
    return other.getType() == ValueType::NilValue;
}

bool SymbolValue::valueEqual(const Value& other) const {
    if (other.getType() != ValueType::SymbolValue) {
        return false;
    }
    auto& otherSymbol = dynamic_cast<const SymbolValue&>(other);
    return value == otherSymbol.value;
}

bool PairValue::valueEqual(const Value& other) const {
    if (other.getType() != ValueType::PairValue) {
        return false;
    }
    auto& otherPair = dynamic_cast<const PairValue&>(other);
    return car->valueEqual(*otherPair.car) && cdr->valueEqual(*otherPair.cdr);
}

bool BuiltinProcValue::valueEqual(const Value& other) const {
    if (other.getType() != ValueType::BuiltinProcValue) {
        return false;
    }
    auto& otherBuiltin = dynamic_cast<const BuiltinProcValue&>(other);
    return func == otherBuiltin.func;
}


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
}
