#ifndef BUILTINS_H
#define BUILTINS_H
// #include <vector>
#include <algorithm>
#include <iostream>
#include <unordered_map>

#include "./error.h"
#include "./value.h"
// #include "./lambda.h"
class Checker {
public:
    static void checkParams(const std::vector<ValuePtr>& params, size_t min,
                            size_t max) {
        if (params.size() < min || params.size() > max) {
            throw LispError("Invalid number of arguments");
        }
    }
    static void checkNumber(const ValuePtr& value) {
        if (!value->isNumber()) {
            throw LispError("Not a number");
        }
    }
    static void checkNumeric(const std::vector<ValuePtr>& params) {
        for (const auto& i : params) {
            checkNumber(i);
        }
    }
    static void checkPair(const ValuePtr& value) {
        if (value->getType() != ValueType::PairValue) {
            throw LispError("Not a pair");
        }
    }
    static void checkPairs(const std::vector<ValuePtr>& params) {
        for (const auto& i : params) {
            checkPair(i);
        }
    }

    static bool checkTypeInList(const std::vector<ValueType>& params,
                                ValueType type) {
        for (const auto& i : params) {
            if (i == type) {
                return true;
            }
        }
        return false;
    }
};

std::unordered_map<std::string, ValuePtr> getBuiltins();
using BuiltinParams = const std::vector<ValuePtr>&;
ValuePtr apply(BuiltinParams params);
ValuePtr eval(BuiltinParams params);
ValuePtr print(BuiltinParams params);
ValuePtr newline(BuiltinParams params);
ValuePtr display(BuiltinParams params);
ValuePtr display_ln(BuiltinParams params);
ValuePtr error(BuiltinParams params);
ValuePtr exit_(BuiltinParams params);

ValuePtr is_atom(BuiltinParams params);
ValuePtr is_boolean(BuiltinParams params);
ValuePtr is_number(BuiltinParams params);
ValuePtr is_integer(BuiltinParams params);
ValuePtr is_list(BuiltinParams params);
ValuePtr is_null(BuiltinParams params);
ValuePtr is_pair(BuiltinParams params);
ValuePtr is_procedure(BuiltinParams params);
ValuePtr is_string(BuiltinParams params);
ValuePtr is_symbol(BuiltinParams params);

ValuePtr add(BuiltinParams params);
ValuePtr subtract(BuiltinParams params);
ValuePtr multiply(BuiltinParams params);
ValuePtr divide(BuiltinParams params);
ValuePtr abs_(BuiltinParams params);
ValuePtr expt(BuiltinParams params);
ValuePtr quotient(BuiltinParams params);
ValuePtr remainder_(BuiltinParams params);
ValuePtr modulo(BuiltinParams params);
ValuePtr eq(BuiltinParams params);
ValuePtr equal(BuiltinParams params);
ValuePtr not_(BuiltinParams params);
ValuePtr lt(BuiltinParams params);
ValuePtr gt(BuiltinParams params);
ValuePtr lte(BuiltinParams params);
ValuePtr gte(BuiltinParams params);
ValuePtr even(BuiltinParams params);
ValuePtr odd(BuiltinParams params);
ValuePtr zero(BuiltinParams params);
ValuePtr cons(BuiltinParams params);
ValuePtr car(BuiltinParams params);
ValuePtr cdr(BuiltinParams params);
ValuePtr list(BuiltinParams params);
ValuePtr length(BuiltinParams params);
ValuePtr append(BuiltinParams params);

#endif  // BUILTINS_H
