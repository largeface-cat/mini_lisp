#ifndef EVAL_ENV_H
#define EVAL_ENV_H
#include <unordered_map>
#include <algorithm>
#include <iterator>
#include "./value.h"
#include "./error.h"
#include "./builtins.h"
class EvalEnv : public std::enable_shared_from_this<EvalEnv>{
private:
    std::unordered_map<std::string, ValuePtr> env{};
    std::shared_ptr<EvalEnv> parent{};

public:
    EvalEnv() {
        env = getBuiltins();
    };
    static std::shared_ptr<EvalEnv> createGlobal();
    ValuePtr eval(ValuePtr expr);
    std::vector<ValuePtr> evalList(const std::vector<ValuePtr>& list);
    ValuePtr apply(const ValuePtr& proc, const std::vector<ValuePtr>& args);
    void define(const std::string& symbol, ValuePtr value);
    ValuePtr lookupBinding(const std::string& symbol);
};
#endif  // EVAL_ENV_H
