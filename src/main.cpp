#include <iostream>
#include <string>

#include "./eval_env.h"
#include "./parser.h"
int argParser(int argc, char** argv) {
    int fileIO = 0b00;
    if (argc > 1) {
        if (argc == 3 && std::string(argv[1]) == "-f") {
            freopen(argv[2], "r", stdin);
            fileIO = 0b10;
        } else if (argc == 3 && std::string(argv[1]) == "-o") {
            freopen(argv[2], "w", stdout);
            fileIO = 0b01;
        } else if (argc == 5 && std::string(argv[1]) == "-f" &&
                   std::string(argv[3]) == "-o") {
            freopen(argv[2], "r", stdin);
            freopen(argv[4], "w", stdout);
            fileIO = 0b11;
        } else if (argc == 5 && std::string(argv[1]) == "-o" &&
                   std::string(argv[3]) == "-f") {
            freopen(argv[4], "r", stdin);
            freopen(argv[2], "w", stdout);
            fileIO = 0b11;
        } else {
            std::cerr << "Usage: " << argv[0] << "\n -f <input_filename>"
                      << "\n -o <output_filename>" << std::endl;
            std::exit(1);
        }
    }
    return fileIO;
}

int main(int argc, char** argv) {
    int fileIO = argParser(argc, argv);
    auto env = EvalEnv::createGlobal();
    while (true) {
        try {
            if (!(fileIO >> 1) || !(fileIO % 2)) std::cout << ">>> ";
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
            if (!(fileIO >> 1)) std::cout << result->toString() << std::endl;
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