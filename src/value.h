#ifndef VALUE_H
#define VALUE_H
#include <memory>
#include <optional>
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
    Value(ValueType type) : type{type} {}

public:
    virtual ~Value() = default;

    ValueType getType() const {
        return type;
    }
    bool isNil() const {
        return type == ValueType::NilValue;
    }
    bool isSelfEvaluating() const {
        return type == ValueType::NumericValue ||
               type == ValueType::BooleanValue ||
               type == ValueType::StringValue;
    }
    virtual std::string toString() const;
    virtual std::vector<ValuePtr> toVector() const;
    virtual std::optional<std::string> asSymbol() const;
    template <typename T>
    T* as() {
        return dynamic_cast<T*>(this);
    }
};

class BooleanValue : public Value {
private:
    bool value;

public:
    BooleanValue(bool value) : Value(ValueType::BooleanValue), value{value} {}

    std::string toString() const override;
    std::vector<ValuePtr> toVector() const override;
    std::optional<std::string> asSymbol() const override;
};

class NumericValue : public Value {
private:
    double value;

public:
    NumericValue(double value) : Value(ValueType::NumericValue), value{value} {}

    std::string toString() const override;
    std::vector<ValuePtr> toVector() const override;
    std::optional<std::string> asSymbol() const override;
};

class StringValue : public Value {
private:
    std::string value;

public:
    StringValue(const std::string& value)
        : Value(ValueType::StringValue), value{value} {}

    std::string toString() const override;
    std::vector<ValuePtr> toVector() const override;
    std::optional<std::string> asSymbol() const override;
};

class NilValue : public Value {
public:
    NilValue() : Value(ValueType::NilValue) {}

    std::string toString() const override;
    std::vector<ValuePtr> toVector() const override;
    std::optional<std::string> asSymbol() const override;
};

class SymbolValue : public Value {
private:
    std::string value;

public:
    SymbolValue(const std::string& value)
        : Value(ValueType::SymbolValue), value{value} {}

    std::string toString() const override;
    std::vector<ValuePtr> toVector() const override;
    std::optional<std::string> asSymbol() const override;
};

class PairValue : public Value {
private:
    ValuePtr car;
    ValuePtr cdr;

public:
    PairValue(ValuePtr car, ValuePtr cdr)
        : Value(ValueType::PairValue), car{car}, cdr{cdr} {}

    std::string toString() const override;
    std::vector<ValuePtr> toVector() const override;
    std::optional<std::string> asSymbol() const override;
};

#endif  // VALUE_H
