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
    [[nodiscard]] virtual std::optional<std::string> asSymbol() const;
    [[nodiscard]] virtual bool isNumber() const {
        return false;
    }
    [[nodiscard]] virtual double asNumber() const {
        throw std::runtime_error("Not a number");
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
};

class StringValue : public Value {
private:
    std::string value;

public:
    explicit StringValue(std::string value)
        : Value(ValueType::StringValue), value{std::move(value)} {}

    [[nodiscard]] std::string toString() const override;
    [[nodiscard]] std::vector<ValuePtr> toVector() const override;
};

class NilValue : public Value {
public:
    NilValue() : Value(ValueType::NilValue) {}

    [[nodiscard]] std::string toString() const override;
    [[nodiscard]] std::vector<ValuePtr> toVector() const override;
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
};

using BuiltinFuncType = ValuePtr(const std::vector<ValuePtr>&);
class BuiltinProcValue : public Value {
private:
    BuiltinFuncType* func;

public:
    explicit BuiltinProcValue(BuiltinFuncType* func)
        : Value(ValueType::SymbolValue), func{func} {}
    [[nodiscard]] std::string toString() const override;
    ValuePtr apply(const std::vector<ValuePtr>& args);
};



#endif  // VALUE_H
