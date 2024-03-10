#include <map>
#include <string>
#include <vector>
#include <iostream>
#include <regex>

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

    std::string mult(const unsigned int& num) {
        std::string output;
        output += move(currPos+1) + ">[<" + std::string(num, '+') + ">-]<";
        return output;
    }
    std::string mult(unsigned int pos1, unsigned int pos2) {
        std::string output;
        int original_pos = currPos;

        output += set_pos(pos1)
                + duplicate_to(original_pos+2)
                + set_pos(pos2)
                + duplicate_to(original_pos + 3);
        pos1 = original_pos+2;
        pos2 = original_pos+3;
        output += set_pos(pos1)
                + "["
                + set_pos(pos2)
                + "["
                + set_pos(original_pos)
                + "+"
                + set_pos(original_pos+1)
                + "+"
                + set_pos(pos2)
                + "-]"
                + set_pos(original_pos+1)
                + "["
                + set_pos(pos2)
                + "+"
                + set_pos(original_pos+1)
                + "-]"
                + set_pos(pos1)
                + "-]"
                + set_pos(pos2)
                + set()
                + set_pos(original_pos);
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

        const int CLOSENESS = 20;
        const int started_pos = currPos;

        std::vector<int> startNums;
        std::map<char, int> charToGroupIndex;

        for (char character : text) {
            bool added = false;

            // Search for suitable startNum
            for (int i = 0; i < startNums.size(); ++i) {
                auto& startNum = startNums[i];
                if (abs(startNum - character) <= CLOSENESS) {
                    added = true;

                    charToGroupIndex[character] = i;
                    break; // Stop searching once a suitable startNum is found
                }
            }
            // If character didn't fit into any startNum, create a new one
            if (!added) {
                startNums.push_back(character); // Correctly initialize with the ASCII value
                charToGroupIndex[character] = startNums.size() - 1;
            }
        }

        // Initialize beginning numbers
        for (auto& startNum : startNums) {
            output += efficient_num(startNum);
            output += set_pos(currPos + 1);
            if (!shortMode) output +=  " //Initialize " + std::to_string(startNum) + "\n";
        }

        // Printing n stuf
        for (auto& character : text) {
            int& startNumPos = charToGroupIndex[character];
            int& startNum = startNums[startNumPos];

            output += set_pos(startNumPos + started_pos);
            int diffBetweenChars = character-startNum;


            // Add/Subtract necessary
            startNum = startNum + diffBetweenChars;
            if (diffBetweenChars > 0) {
                output += std::string(diffBetweenChars, '+');
                if (!shortMode) output += " //Add " + std::to_string(diffBetweenChars) + "\n";
            } else if (diffBetweenChars < 0) {
                output += std::string(-diffBetweenChars, '-');
                if (!shortMode) output += " //Subtract " + std::to_string(-diffBetweenChars) + "\n";
            }
            output += ".";
            if (!shortMode) output += " //Print\n";
        }

        // Delete initialized nums
        for (int i = currPos; i > started_pos - 1; --i) {
            output += set_pos(i); // Go back one
            output += set(); // Set to 0
        }
        if (!shortMode) output += " //Reset initial values to zero\n";

        return output;
    }

    static std::string make_sure_bf_neat(std::string text, int wrap = 0) {
        // Fix going back n forth
        text = std::regex_replace(text, std::regex("<>|><"), "");


        if (wrap != 0) {
            for (int i = 0; i < text.length(); ++i) {
                if (i % wrap == 0 && i != 0) {
                    text.insert(i, "\n");
                }
            }
        }

        return text;
    }
};

int main() {
    Brainfun bf(true);

    std::vector<std::function<std::string()>> instructions = {
            [&](){return bf.set_pos(5);},
            [&](){return bf.efficient_text("Hello *meow*");},

    };

    std::cout << ">++++++++[<+++++++++>-]>+++++++[<+++++++++++++++>-]>+++[<+++++++++++>-]<<<.>.>.[-]<[-]<[-]\n";


    for (const auto& instruction : instructions) {
        std::cout << Brainfun::make_sure_bf_neat(instruction());
        if (!bf.shortMode) std::cout << "\n\n";
    }

    return 0;
}