#ifndef LAMBDA_H
#define LAMBDA_H
#include "./eval_env.h"
class LambdaValue : public Value {
private:
    std::vector<std::string> params;
    std::vector<ValuePtr> body;
    std::shared_ptr<EvalEnv> parent_env;

public:
    LambdaValue(std::vector<std::string> params, std::vector<ValuePtr> body,
                std::shared_ptr<EvalEnv> env)
        : Value(ValueType::SymbolValue),
          params{std::move(params)},
          body{std::move(body)},
          parent_env{std::move(env)} {}
    [[nodiscard]] std::string toString() const override;
    ValuePtr apply(const std::vector<ValuePtr>& args);
};
#endif  // LAMBDA_H
