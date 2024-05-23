#include "./builtins.h"
std::unordered_map<std::string, ValuePtr> getBuiltins() {
    return {
        {"print", std::make_shared<BuiltinProcValue>(print)},
        {"+", std::make_shared<BuiltinProcValue>(add)},

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