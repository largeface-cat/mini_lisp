#ifndef BUILTINS_H
#define BUILTINS_H
#include <vector>
#include <unordered_map>
#include <iostream>
#include "./value.h"
#include "./error.h"
class Checker{
    public:
    static void checkParams(const std::vector<ValuePtr>& params, size_t min, size_t max){
        if(params.size() < min || params.size() > max){
            throw LispError("Invalid number of arguments");
        }
    }
    static void checkNumber(const ValuePtr& value){
        if(!value->isNumber()){
            throw LispError("Not a number");
        }
    }
    static void checkNumeric(const std::vector<ValuePtr>& params){
        for(const auto& i : params){
            checkNumber(i);
        }
    }
    static void checkPair(const ValuePtr& value){
        if(value->getType() != ValueType::PairValue){
            throw LispError("Not a pair");
        }
    }
    static void checkPairs(const std::vector<ValuePtr>& params){
        for(const auto& i : params){
            checkPair(i);
        }
    }
};

std::unordered_map<std::string, ValuePtr> getBuiltins();

ValuePtr print(const std::vector<ValuePtr>& params);
ValuePtr newline(const std::vector<ValuePtr>& params);
ValuePtr display(const std::vector<ValuePtr>& params);
ValuePtr exit_(const std::vector<ValuePtr>& params);
ValuePtr add(const std::vector<ValuePtr>& params);
ValuePtr subtract(const std::vector<ValuePtr>& params);
ValuePtr multiply(const std::vector<ValuePtr>& params);
ValuePtr divide(const std::vector<ValuePtr>& params);
ValuePtr abs_(const std::vector<ValuePtr>& params);
ValuePtr expt(const std::vector<ValuePtr>& params);
ValuePtr quotient(const std::vector<ValuePtr>& params);
ValuePtr remainder_(const std::vector<ValuePtr>& params);
ValuePtr modulo(const std::vector<ValuePtr>& params);
ValuePtr eq(const std::vector<ValuePtr>& params);
ValuePtr lt(const std::vector<ValuePtr>& params);
ValuePtr gt(const std::vector<ValuePtr>& params);
ValuePtr lte(const std::vector<ValuePtr>& params);
ValuePtr gte(const std::vector<ValuePtr>& params);
ValuePtr even(const std::vector<ValuePtr>& params);
ValuePtr odd(const std::vector<ValuePtr>& params);
ValuePtr zero(const std::vector<ValuePtr>& params);
ValuePtr cons(const std::vector<ValuePtr>& params);
ValuePtr car(const std::vector<ValuePtr>& params);
ValuePtr cdr(const std::vector<ValuePtr>& params);
ValuePtr list(const std::vector<ValuePtr>& params);
ValuePtr length(const std::vector<ValuePtr>& params);
ValuePtr append(const std::vector<ValuePtr>& params);

#endif  // BUILTINS_H
