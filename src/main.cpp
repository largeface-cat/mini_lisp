#include <iostream>
#include <string>

#include "./eval_env.h"
#include "./parser.h"
int main(int argc, char** argv) {
    bool from_file = false;
    if (argc > 1) {
        if (argc != 3 || std::string(argv[1]) != "-f") {
            std::cerr << "Usage: " << argv[0] << " -f <filename>" << std::endl;
            return 1;
        }
        freopen(argv[2], "r", stdin);
        from_file = true;
    }
    auto env = EvalEnv::createGlobal();
    while (true) {
        try {
            if (!from_file) std::cout << ">>> ";
            std::string line;
            std::getline(std::cin, line);
            if (std::cin.eof()) {
                std::exit(0);
            }
            if (line.empty()) {
                continue;
            }
            auto tokens = Tokenizer::tokenize(line);
            Parser parser(std::move(tokens));
            auto value = parser.parse();
            auto result = env->eval(std::move(value));
            if (!from_file) std::cout << result->toString() << std::endl;
        } catch (std::runtime_error& e) {
            std::cerr << "Error: " << e.what() << std::endl;
        }
    }
}

// #include "rjsj_test.hpp"
// struct TestCtx {
//     std::shared_ptr<EvalEnv> env = EvalEnv::createGlobal();
//     std::string eval(const std::string& input) {
//         auto tokens = Tokenizer::tokenize(input);
//         Parser parser(std::move(tokens));
//         auto value = parser.parse();
//         auto result = env->eval(std::move(value));
//         return result->toString();
//     }
// };
// int main() {
//     RJSJ_TEST(TestCtx, Lv2, Lv3, Lv4, Lv5, Lv5Extra, Lv6, Lv7, Lv7Lib, Sicp);
// }