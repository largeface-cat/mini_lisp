#ifndef BUILTINS_H
#define BUILTINS_H
#include <vector>
#include <unordered_map>
#include <iostream>
#include "./value.h"
#include "./error.h"
std::unordered_map<std::string, ValuePtr> getBuiltins();
ValuePtr print(const std::vector<ValuePtr>& params);
ValuePtr add(const std::vector<ValuePtr>& params);
ValuePtr subtract(const std::vector<ValuePtr>& params);
ValuePtr multiply(const std::vector<ValuePtr>& params);
ValuePtr divide(const std::vector<ValuePtr>& params);
ValuePtr eq(const std::vector<ValuePtr>& params);
ValuePtr lt(const std::vector<ValuePtr>& params);
ValuePtr gt(const std::vector<ValuePtr>& params);
ValuePtr lte(const std::vector<ValuePtr>& params);
ValuePtr gte(const std::vector<ValuePtr>& params);
ValuePtr cons(const std::vector<ValuePtr>& params);
ValuePtr car(const std::vector<ValuePtr>& params);
ValuePtr cdr(const std::vector<ValuePtr>& params);
ValuePtr list(const std::vector<ValuePtr>& params);
ValuePtr length(const std::vector<ValuePtr>& params);

#endif  // BUILTINS_H
