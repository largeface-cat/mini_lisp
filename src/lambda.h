#ifndef MINI_LISP_LAMBDA_H
#define MINI_LISP_LAMBDA_H
#include "./eval_env.h"
class LambdaValue : public Value {
private:
    std::vector<std::string> params;
    std::vector<ValuePtr> body;
    std::shared_ptr<EvalEnv> env;

public:
    LambdaValue(std::vector<std::string> params, std::vector<ValuePtr> body,
                std::shared_ptr<EvalEnv> env)
        : Value(ValueType::SymbolValue),
          params{std::move(params)},
          body{std::move(body)},
          env{std::move(env)} {}
    [[nodiscard]] std::string toString() const override;
};
#endif  // MINI_LISP_LAMBDA_H
