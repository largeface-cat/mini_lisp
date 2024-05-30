#ifndef FORMS_H
#define FORMS_H
#include "./value.h"
#include <unordered_map>
#include "./eval_env.h"

using SpecialFormType = ValuePtr(const std::vector<ValuePtr>&, EvalEnv&);
extern const std::unordered_map<std::string, SpecialFormType*> SPECIAL_FORMS;
ValuePtr defineForm(const std::vector<ValuePtr>& args, EvalEnv& env);
ValuePtr quoteForm(const std::vector<ValuePtr>& args,
                   [[maybe_unused]] EvalEnv& env);
ValuePtr ifForm(const std::vector<ValuePtr>& args, EvalEnv& env);
ValuePtr andForm(const std::vector<ValuePtr>& args, EvalEnv& env);
ValuePtr orForm(const std::vector<ValuePtr>& args, EvalEnv& env);
ValuePtr lambdaForm(const std::vector<ValuePtr>& args, EvalEnv& env);
ValuePtr condForm(const std::vector<ValuePtr>& args, EvalEnv& env);
ValuePtr letForm(const std::vector<ValuePtr>& args, EvalEnv& env);
ValuePtr beginForm(const std::vector<ValuePtr>& args, EvalEnv& env);
ValuePtr quasiQuoteForm(const std::vector<ValuePtr>& args, EvalEnv& env);
ValuePtr unquoteForm([[maybe_unused]] const std::vector<ValuePtr>& args, EvalEnv& env);
#endif  // FORMS_H
