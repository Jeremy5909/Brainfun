#include <iostream>

struct Brainfun {
private:
    std::string negate_arrows(const std::string& arrows) {
        std::string result = arrows;
        for(auto& arrow : result) {
            if (arrow == '>') {
                arrow = '<';
            } else if (arrow == '<') {
                arrow = '>';
            }
        }
        return result;
    }

    bool isPrime(unsigned long long int num) {
        for (int i = 2; i*i <= num ; i++) {
            if (num % i == 0) {
                return false;
                break;
            }
        }
        return true;
    }

public:
    std::string efficient_num(int num) {
        if(num < 15) {
            return std::string(num, '+');
        } else {
            // Get two closest primes
            int additional = 0;
            if (isPrime(num)) {
                additional = 1;
                num = num - 1;
            }
            int firstClosest = (int) sqrt(num);
            while (num % firstClosest != 0) {
                firstClosest = firstClosest - 1;
            }
            int secondClosest = (int) num/firstClosest;

            return ">" + std::string(firstClosest, '+') + "[<" + std::string(secondClosest, '+') + ">-]<" + std::string(additional, '+');
        }
    }
    std::string add(const int& num) {
        return std::string(num, '+');
    }
    std::string subtract(const int& num) {
        return std::string(num, '-');
    }
    std::string mult(const int& currPos, const int& num) {
        std::string output;
        output += move(currPos, currPos+1) + ">[<" + std::string(num, '+') + ">-]<";
        return output;
    }
    std::string set(const int& num = 0) {
        return "[-]" + std::string(num, '+');
    }
    std::string set_pos(int& currPos, int pos) {
        std::string output;
        if (pos > currPos) {
            output = std::string(pos - currPos, '>');
        } else if (pos < currPos) {
            output = std::string(currPos - pos, '<');
        }

        currPos = pos;
        return output;
    }

    std::string move(int currPos, int target) {
        return "[" + set_pos(currPos, target) + "+" + negate_arrows(set_pos(currPos, target)) + "<-]";
    }

    std::string plainText(std::string text) {
        std::string output;
        for (auto& character : text) {
            output += efficient_num(int(character)) + "." + set();
        }
        return output;
    }
};

int main() {
    Brainfun bf;

    int currPos = 1;

    std::vector<std::function<std::string()>> instructions = {
            [&]() { return bf.efficient_num(5); },
            [&]() { return bf.mult(currPos, 4); },
    };


    for (const auto& instruction : instructions) {
        std::cout << instruction();
    }

    bf.plainText("hello");

    return 0;
}
