#include <iostream>
#include <string>

#include "./parser.h"
#include "eval_env.h"
using namespace std::literals;
//int main() {
//    EvalEnv env;
//    while (true) {
//        try {
//            std::cout << ">>> ";
//            std::string line;
//            std::getline(std::cin, line);
//            if (std::cin.eof()) {
//                std::exit(0);
//            }
//            if (line.empty()) {
//                continue;
//            }
//            auto tokens = Tokenizer::tokenize(line);
//            Parser parser(std::move(tokens));
//            auto value = parser.parse();
//            auto result = env.eval(std::move(value));
//            std::cout << result->toString() << std::endl;
//            for (auto& token : tokens) {
//                std::cout << *token << std::endl;
//            }
//        } catch (std::runtime_error& e) {
//            std::cerr << "Error: " << e.what() << std::endl;
//        }
//    }
//}

#include "rjsj_test.hpp"
struct TestCtx {
    EvalEnv env;
    std::string eval(std::string input) {
        auto tokens = Tokenizer::tokenize(input);
        Parser parser(std::move(tokens));
        auto value = parser.parse();
        auto result = env.eval(std::move(value));
        return result->toString();
    }
};
int main() {
//    RJSJ_TEST(TestCtx, Lv2, Lv3, Lv4, Lv5);
    // 若你已完成该功能 * 和 > 内置过程的实现，则可测试 Lv5Extra 测试集：
     RJSJ_TEST(TestCtx, Lv2, Lv3, Lv4, Lv5, Lv5Extra);
}