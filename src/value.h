#ifndef VALUE_H
#define VALUE_H
#include <memory>
#include <optional>
#include <utility>
#include <vector>

enum class ValueType {
    BooleanValue,
    NumericValue,
    StringValue,
    NilValue,
    SymbolValue,
    PairValue,
    BuiltinProcValue,
    LambdaValue,
};
class Value;
using ValuePtr = std::shared_ptr<Value>;
class Value {
private:
    ValueType type;

protected:
    explicit Value(ValueType type) : type{type} {}

public:
    virtual ~Value() = default;

    [[nodiscard]] ValueType getType() const {
        return type;
    }
    [[nodiscard]] bool isNil() const {
        return type == ValueType::NilValue;
    }
    [[nodiscard]] bool isSelfEvaluating() const {
        return type == ValueType::NumericValue ||
               type == ValueType::BooleanValue ||
               type == ValueType::StringValue;
    }
    [[nodiscard]] virtual std::string toString() const;
    [[nodiscard]] virtual std::vector<ValuePtr> toVector() const;
    [[nodiscard]] virtual bool valueEqual(const Value& other) const;
    [[nodiscard]] virtual std::optional<std::string> asSymbol() const;
    [[nodiscard]] virtual bool isNumber() const {
        return false;
    }
    [[nodiscard]] virtual double asNumber() const {
        throw std::runtime_error("Not a number");
    }
    virtual ValuePtr apply(const std::vector<ValuePtr>& args) {
        throw std::runtime_error("Not a procedure");
    }
    template <typename T>
    T* as() {
        return dynamic_cast<T*>(this);
    }
};

class BooleanValue : public Value {
private:
    bool value;

public:
    explicit BooleanValue(bool value)
        : Value(ValueType::BooleanValue), value{value} {}

    [[nodiscard]] std::string toString() const override;
    [[nodiscard]] std::vector<ValuePtr> toVector() const override;
    [[nodiscard]] bool asBool() const {
        return value;
    }
    [[nodiscard]] bool valueEqual(const Value& other) const override;
};

class NumericValue : public Value {
private:
    double value;

public:
    explicit NumericValue(double value)
        : Value(ValueType::NumericValue), value{value} {}

    [[nodiscard]] std::string toString() const override;
    [[nodiscard]] std::vector<ValuePtr> toVector() const override;
    [[nodiscard]] bool isNumber() const override {
        return true;
    }
    [[nodiscard]] double asNumber() const override {
        return value;
    }
    [[nodiscard]] bool valueEqual(const Value& other) const override;
};

class StringValue : public Value {
private:
    std::string value;

public:
    explicit StringValue(std::string value)
        : Value(ValueType::StringValue), value{std::move(value)} {}

    [[nodiscard]] std::string toString() const override;
    [[nodiscard]] std::string getValue() const {
        return value;
    }
    [[nodiscard]] std::vector<ValuePtr> toVector() const override;
    [[nodiscard]] bool valueEqual(const Value& other) const override;
};

class NilValue : public Value {
public:
    NilValue() : Value(ValueType::NilValue) {}

    [[nodiscard]] std::string toString() const override;
    [[nodiscard]] std::vector<ValuePtr> toVector() const override;
    [[nodiscard]] bool valueEqual(const Value& other) const override;
};

class SymbolValue : public Value {
private:
    std::string value;

public:
    explicit SymbolValue(std::string value)
        : Value(ValueType::SymbolValue), value{std::move(value)} {}

    [[nodiscard]] std::string toString() const override;
    [[nodiscard]] std::vector<ValuePtr> toVector() const override;
    [[nodiscard]] std::optional<std::string> asSymbol() const override;
    [[nodiscard]] bool valueEqual(const Value& other) const override;
};

class PairValue : public Value {
private:
    ValuePtr car;
    ValuePtr cdr;

public:
    PairValue(ValuePtr car, ValuePtr cdr)
        : Value(ValueType::PairValue),
          car{std::move(car)},
          cdr{std::move(cdr)} {}

    explicit PairValue(std::vector<ValuePtr> values)
        : Value(ValueType::PairValue) {
        if (values.empty()) {
            throw std::runtime_error("Empty list");
        }
        car = values[0];
        cdr = std::make_shared<NilValue>();
        ValuePtr current = nullptr;
        for (size_t i = 1; i < values.size(); i++) {
            if (current == nullptr) {
                current = std::make_shared<PairValue>(
                    values[i], std::make_shared<NilValue>());
                cdr = current;
            } else {
                current->as<PairValue>()->cdr = std::make_shared<PairValue>(
                    values[i], std::make_shared<NilValue>());
                current = current->as<PairValue>()->cdr;
            }
        }
    }

    [[nodiscard]] std::string toString() const override;
    [[nodiscard]] std::vector<ValuePtr> toVector() const override;
    [[nodiscard]] ValuePtr getCar() const {
        return car;
    }
    [[nodiscard]] ValuePtr getCdr() const {
        return cdr;
    }
    void setCar(ValuePtr value) {
        car = std::move(value);
    }
    void setCdr(ValuePtr value) {
        cdr = std::move(value);
    }
    [[nodiscard]] bool valueEqual(const Value& other) const override;
};

using BuiltinFuncType = ValuePtr(const std::vector<ValuePtr>&);
class BuiltinProcValue : public Value {
private:
    BuiltinFuncType* func;

public:
    explicit BuiltinProcValue(BuiltinFuncType* func)
        : Value(ValueType::BuiltinProcValue), func{func} {}
    [[nodiscard]] std::string toString() const override;
    [[nodiscard]] bool valueEqual(const Value& other) const override;
    ValuePtr apply(const std::vector<ValuePtr>& args) override;
};

class EvalEnv;
class LambdaValue : public Value {
private:
    std::vector<std::string> params;
    std::vector<ValuePtr> body;
    std::shared_ptr<EvalEnv> parent_env;

public:
    LambdaValue(std::vector<std::string> params, std::vector<ValuePtr> body,
                std::shared_ptr<EvalEnv> env)
        : Value(ValueType::LambdaValue),
          params{std::move(params)},
          body{std::move(body)},
          parent_env{std::move(env)} {}
    [[nodiscard]] std::string toString() const override;
    [[nodiscard]] bool valueEqual(const Value& other) const override;
    ValuePtr apply(const std::vector<ValuePtr>& args) override;
};
#endif  // VALUE_H
