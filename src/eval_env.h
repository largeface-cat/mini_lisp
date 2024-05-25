#ifndef EVAL_ENV_H
#define EVAL_ENV_H
#include <algorithm>
#include <unordered_map>

#include "./builtins.h"
#include "./value.h"
class EvalEnv : public std::enable_shared_from_this<EvalEnv> {
private:
    std::unordered_map<std::string, ValuePtr> env{};
    std::shared_ptr<EvalEnv> parent{};
    EvalEnv() = default;

public:
    static std::shared_ptr<EvalEnv> createGlobal();
    static std::shared_ptr<EvalEnv> createNew(
        const std::vector<std::string>& params,
        const std::vector<ValuePtr>& args);
    void setParent(std::shared_ptr<EvalEnv>& p);
    void setEnv(const std::unordered_map<std::string, ValuePtr>& env_map);
    ValuePtr eval(ValuePtr expr);
    std::vector<ValuePtr> evalList(const std::vector<ValuePtr>& list);
    ValuePtr apply(const ValuePtr& proc, const std::vector<ValuePtr>& args);
    void define(const std::string& symbol, ValuePtr value);
    ValuePtr lookupBinding(const std::string& symbol);
};

extern const std::shared_ptr<EvalEnv> GLOBAL_ENV;
#endif  // EVAL_ENV_H
