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

    bool isPrime(unsigned int num) {
        for (int i = 2; i*i <= num ; i++) {
            if (num % i == 0) {
                return false;
                break;
            }
        }
        return true;
    }
public:
    // TODO add second parameter for things like effecient_num for where to put the temp second num
    std::string efficient_num(unsigned int num) {
        // TODO make go both up and down and choose which is best
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

    // TODO make number that adds or subtractes based on input and effeciently w function

    // TODO make take pos ?
    std::string mult(const unsigned int& currPos, const unsigned int& num) {
        std::string output;
        output += move(currPos, currPos+1) + ">[<" + std::string(num, '+') + ">-]<";
        return output;
    }
    std::string set(const unsigned int& num = 0) {
        return "[-]" + std::string(num, '+');
    }
    std::string set_pos(unsigned int& currPos, unsigned int pos) {
        std::string output;
        if (pos > currPos) {
            output = std::string(pos - currPos, '>');
        } else if (pos < currPos) {
            output = std::string(currPos - pos, '<');
        }

        currPos = pos;
        return output;
    }

    std::string move(unsigned int currPos, int target) {
        return "[" + set_pos(currPos, target) + "+" + negate_arrows(set_pos(currPos, target)) + "<-]";
    }

    std::string raw_text(std::string text) {
        // TODO make way more efficient by getting average and only adding necessary
        std::string output;
        for (auto& character : text) {
            output += efficient_num(int(character)) + "." + set();
        }
        return output;
    }


    // Make sure to have room one space after target
    std::string duplicate_to(unsigned int currPos, unsigned int targetPos) {
        // TODO make it work even when one space after target isn't clean
        std::string output;
        unsigned int startPos = currPos;

        output += "["
                + set_pos(currPos, targetPos)
                + "+"
                + set_pos(currPos, currPos + 1)
                + "+"
                + set_pos(currPos, startPos)
                + "-]"
                + set_pos(currPos, targetPos + 1)
                + "["
                + set_pos(currPos, startPos)
                + "+"
                + set_pos(currPos, targetPos + 1)
                + "-]"
                + set_pos(currPos, startPos);
        return output;
    }

    // TODO Have the type of curr. pos be template of either value or ref
    // TODO have one to compare value of two positions
};

int main() {
    Brainfun bf;

    unsigned int currPos = 1;

    const int NAME = 1;

    std::vector<std::function<std::string()>> instructions = {
            [&]() { return bf.raw_text("Hello!"); },
    };


    for (const auto& instruction : instructions) {
        std::cout << instruction();
    }

    return 0;
}