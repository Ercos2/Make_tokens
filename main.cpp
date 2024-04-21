#include <iostream>
#include <vector>
#include <memory>
#include <variant>
#include <unordered_map>

// (1 + 2) * 3 / 4 + 5 * (6 - 7)

// 1. Токенизация
// 2. Парсер (построение дерева разбора выражения)

// +, -, *, /, %

struct OpeningBracket {
    void get_name() {
        std::cout << "(\n";
    }
};

struct ClosingBracket {
    void get_name() {
        std::cout << ")\n";
    }
};

struct Number {
    int value;
    void get_name() {
        std::cout << "number " << value << "\n";
    }
};

struct UnknownToken {
    std::string value;
    UnknownToken(std::string new_value) : value(new_value) {};
    void get_name() {
        std::cout << "unknown token " << value << "\n";
    }
};

struct MinToken {
    void get_name() {
        std::cout << "min token\n";
    }
};

struct MaxToken {
    void get_name() {
        std::cout << "max token\n";
    }
};

struct AbsToken {
    void get_name() {
        std::cout << "abs token\n";
    }
};

struct SqrToken {
    void get_name() {
        std::cout << "sqr token\n";
    }
};

struct Plus {
    void get_name() {
        std::cout << "+ token\n";
    }
};

struct Minus {
    void get_name() {
        std::cout << "- token\n";
    }
};

struct Multiply {
    void get_name() {
        std::cout << "* token\n";
    }
};

struct Modulo {
    void get_name() {
        std::cout << "% token\n";
    }
};

struct Divide {
    void get_name() {
        std::cout << "/ token\n";
    }
};

struct CallPrintName {
    void operator()(OpeningBracket& d) { d.get_name(); }
    void operator()(ClosingBracket& d) { d.get_name(); }
    void operator()(Number& d) { d.get_name(); }
    void operator()(UnknownToken& d) { d.get_name(); }
    void operator()(MinToken& d) { d.get_name(); }
    void operator()(MaxToken& d) { d.get_name(); }
    void operator()(AbsToken& d) { d.get_name(); }
    void operator()(SqrToken& d) { d.get_name(); }
    void operator()(Plus& d) { d.get_name(); }
    void operator()(Minus& d) { d.get_name(); }
    void operator()(Multiply& d) { d.get_name(); }
    void operator()(Modulo& d) { d.get_name(); }
    void operator()(Divide& d) { d.get_name(); }
};


using Token = std::variant<OpeningBracket, ClosingBracket, Number, UnknownToken, MinToken, MaxToken, AbsToken, SqrToken, Plus, Minus, Multiply, Modulo,  Divide>;

// 1234

const std::unordered_map<char, Token> kSymbol2Token {
        {'(', OpeningBracket{}}, {')', ClosingBracket{}}, {'+', Plus{}}, {'-', Minus{}}, {'*', Multiply{}}, {'%', Modulo{}}, {'/', Divide{}}};

const std::unordered_map<std::string, Token> StrTokens {
        {"min", MinToken{}}, {"max", MaxToken{}}, {"abs", AbsToken{}}, {"sqr", SqrToken{}}};

int ToDigit(unsigned char symbol) {
    return symbol - '0';
}

Number ParseNumber(const std::string& input, size_t& pos) {
    int value = 0;
    auto symbol = static_cast<unsigned char>(input[pos]);
    while (std::isdigit(symbol)) {
        value = value * 10 + ToDigit(symbol);
        if (pos == input.size() - 1) {
            ++pos;
            break;
        }
        symbol = static_cast<unsigned char>(input[++pos]);
    }
    return Number{value};
}

Token ParseName(const std::string& input, size_t& pos) {
    std::string unknown_str = "";
    bool unknown_str_flag = false;
    const size_t size = input.size();

    while (pos < size) {
        const auto symbol = static_cast<unsigned char>(input[pos]);
        if (std::isspace(symbol)) {
            unknown_str_flag = true;
        } else if (std::isdigit(symbol)) {
            unknown_str_flag = true;
        } else if (auto it = kSymbol2Token.find(symbol); it != kSymbol2Token.end()) {
            unknown_str_flag = true;
        } else {
            unknown_str += symbol;
            ++pos;
            unknown_str_flag = false;
        }
        //while the unknown characters continue, they are entered into the string,
        // and the meeting of the first known one makes it clear that the unknown word is finished
        // and we can check if we know it
        // and if line is over it's probably time to read
        if (unknown_str_flag || pos == size) {
            //std::cout << "new_str " << unknown_str << "\n";
            if (auto it = StrTokens.find(unknown_str); it != StrTokens.end()) {
                return it->second;
            } else {
                return UnknownToken(unknown_str);
            }
        }
    }
    return UnknownToken("Can't read line");
}

std::vector<Token> Tokenize(const std::string& input) {
    std::vector<Token> tokens;
    const size_t size = input.size();
    size_t pos = 0;

    while (pos < size) {
        const auto symbol = static_cast<unsigned char>(input[pos]);
        //std::cout << symbol << "\n";
        if (std::isspace(symbol)) {
            ++pos;
        } else if (std::isdigit(symbol)) {
            tokens.emplace_back(ParseNumber(input, pos));
        } else if (auto it = kSymbol2Token.find(symbol); it != kSymbol2Token.end()) {
            tokens.emplace_back(it->second);
            ++pos;
        } else {
            tokens.push_back(ParseName(input, pos));
        }
    }
    return tokens;
}

int main () {

    while(true) {
        std::string str_from_cin;
        std::cout << "Enter a string\n";
        //I really wanted to read spaces
        //std::cin >> str_from_cin;
        std::getline(std::cin, str_from_cin);
        std::cout << str_from_cin << "\n";
        std::vector<Token> tokens = Tokenize(str_from_cin);
        for (auto &token: tokens) {
            std::visit(CallPrintName{}, token);

        }
    }
}