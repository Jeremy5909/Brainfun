#include <map>
#include <string>
#include <vector>
#include <iostream>

struct Brainfun {
private:
    int currPos = 1;

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
            }
        }
        return true;
    }
public:
    bool shortMode;

    Brainfun(bool shortMode = true) : shortMode(shortMode) {}

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
    std::string mult(const unsigned int& num) {
        std::string output;
        output += move(currPos+1) + ">[<" + std::string(num, '+') + ">-]<";
        return output;
    }
    std::string set(const unsigned int& num = 0) {
        return "[-]" + std::string(num, '+');
    }
    std::string set_pos(unsigned int pos) {
        std::string output;
        if (pos > currPos) {
            output = std::string(pos - currPos, '>');
        } else if (pos < currPos) {
            output = std::string(currPos - pos, '<');
        }

        currPos = pos;
        return output;
    }

    std::string move(int target) {
        return "[" + set_pos(target) + "+" + negate_arrows(set_pos(target)) + "<-]";
    }

    std::string raw_text(const std::string& text) {
        // TODO make way more efficient by getting average and only adding necessary
        std::string output;
        for (auto& character : text) {
            output += efficient_num(int(character)) + "." + set(); // Putitin , print , reset
            if (!shortMode) output += std::string(" //Char ") + character + '\n'; // Comment
        }
        return output;
    }

    // Make sure to have room one space after target
    std::string duplicate_to(unsigned int targetPos) {
        // TODO make it work even when one space after target isn't clean
        std::string output;
        unsigned int startPos = currPos;

        output = "["
                + set_pos(targetPos)
                + "+"
                + set_pos(currPos + 1)
                + "+"
                + set_pos(startPos)
                + "-]"
                + set_pos(targetPos + 1)
                + "["
                + set_pos(startPos)
                + "+"
                + set_pos(targetPos + 1)
                + "-]"
                + set_pos(startPos);
        return output;
    }

    std::string efficient_text(const std::string& text) {
        std::string output;

        std::vector<std::vector<int>> groups;
        std::map<char, int> charToGroupIndex;

        //
        for (char character : text) {
            bool added = false;
            for (int i = 0; i < groups.size(); ++i) {
                auto& group = groups[i];
                if (abs(group[0] - character) <= 26) {
                    added = true;

                    group.push_back(static_cast<int>(character));
                    charToGroupIndex[character] = i;
                    break; // Stop searching once a suitable group is found
                }
            }
            // If character didn't fit into any group, create a new one
            if (!added) {
                groups.push_back(std::vector<int> {static_cast<int>(character)}); // Correctly initialize with the ASCII value
                charToGroupIndex[character] = groups.size() - 1;
            }
        }

        // Set beginning nums
        for (auto& group : groups) {
            output += efficient_num(group[0]);
            output += set_pos(currPos + 1);
            if (!shortMode) output +=  " //Initialize " + std::to_string(group[0]) + "\n";
        }


        for (auto& character : text) {
            // Rework groups so they're just one int and not vector
            int groupPos = charToGroupIndex[character];
            std::vector<int> currGroup = groups[groupPos];

            output += set_pos(groupPos + 1);
            int diffBetweenChars = static_cast<int>(character) - currGroup[0];


            // Add/Subtract necessary
            currGroup[0] = currGroup[0] + diffBetweenChars;
            if (diffBetweenChars > 0) {
                output += std::string(diffBetweenChars, '+');
            } else if (diffBetweenChars < 0) {
                output += std::string(abs(diffBetweenChars), '-');
            }
            output += ".";
        }

        return output;
    }

    // TODO Have the type of curr. pos be template of either value or ref
};

int main() {
    Brainfun bf(false);

    std::vector<std::function<std::string()>> instructions = {
            [&]() {return bf.efficient_text("Hello World!");}
    };


    for (const auto& instruction : instructions) {
        std::cout << instruction();
        if (!bf.shortMode) std::cout << "\n\n";
    }

    return 0;
}