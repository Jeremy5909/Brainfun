#include <iostream>
#import <sstream>
#import <fstream>
#include <string>
#include <map>
#include <vector>
#include <regex>

struct Brainfun {
private:
    unsigned int curr_pos = 1;

    std::string NegateArrows(const std::string& arrows) {
        std::string output = arrows;
        for(auto& arrow : output) {
            if (arrow == '>') {
                arrow = '<';
            } else if (arrow == '<') {
                arrow = '>';
            }
        }
        return output;
    }

    bool IsPrime(unsigned int num) {
        for (int i = 2; i*i <= num ; i++) {
            if (num % i == 0) {
                return false;
            }
        }
        return true;
    }
public:
    bool short_mode;

    int getPos() {
        return curr_pos;
    }

    Brainfun(bool shortMode = true) : short_mode(shortMode) {}

    // TODO add second parameter for things like effecient_num for where to put the temp second num
    std::string Add(int num) {
        std::string output;
        // TODO make go both up and down and choose which is best
        if(abs(num) < 15) {
            if (num > 0) {
                output = std::string(num, '+');
            } else if (num < 0) {
                output = std::string(num, '-');
            }
        } else {
            // Get two closest primes
            int additional = 0;
            int tempNum = abs(num);
            if (IsPrime(tempNum)) {
                additional = 1;
                tempNum -= 1;
            }
            int first_closest = static_cast<int>(sqrt(tempNum));
            while (num % first_closest != 0) {
                first_closest = first_closest - 1;
            }
            int second_closest = static_cast<int>(tempNum/first_closest);

            output = SetPos(curr_pos+1) + std::string(first_closest, '+') + "[" + SetPos(curr_pos-1);

            if (num > 0) {
                output += std::string(second_closest, '+');
            } else if (num < 0) {
                output += std::string(second_closest, '-');
            }

            output += SetPos(curr_pos+1) + "-]" + SetPos(curr_pos - 1) + std::string(additional, '+');
        }
        return output;
    }

    // TODO make number that adds or subtractes based on input and effeciently w function

    std::string Mult(const unsigned int& num) {
        std::string output;
        output += Move(curr_pos + 1) + ">[<" + std::string(num, '+') + ">-]<";
        return output;
    }
    std::string Mult(unsigned int pos1, unsigned int pos2) {
        std::string output;
        int original_pos = curr_pos;

        output += SetPos(pos1)
                  + DuplicateTo(original_pos + 2)
                  + SetPos(pos2)
                  + DuplicateTo(original_pos + 3);
        pos1 = original_pos+2;
        pos2 = original_pos+3;
        output += SetPos(pos1)
                  + "["
                  + SetPos(pos2)
                  + "["
                  + SetPos(original_pos)
                  + "+"
                  + SetPos(original_pos + 1)
                  + "+"
                  + SetPos(pos2)
                  + "-]"
                  + SetPos(original_pos + 1)
                  + "["
                  + SetPos(pos2)
                  + "+"
                  + SetPos(original_pos + 1)
                  + "-]"
                  + SetPos(pos1)
                  + "-]"
                  + SetPos(pos2)
                  + Set()
                    + SetPos(original_pos);
        return output;
    }
    std::string Set(const unsigned int& num = 0) {
        return "[-]" + std::string(num, '+');
    }
    std::string SetPos(unsigned int pos) {
        std::string output;
        if (pos > curr_pos) {
            output = std::string(pos - curr_pos, '>');
        } else if (pos < curr_pos) {
            output = std::string(curr_pos - pos, '<');
        }

        curr_pos = pos;
        return output;
    }

    std::string Move(int target) {
        return "[" + SetPos(target) + "+" + NegateArrows(SetPos(target)) + "<-]";
    }

    std::string SlowText(const std::string& text) {
        // TODO make way more efficient by getting average and only adding necessary
        std::string output;
        for (auto& character : text) {
            output += Add(int(character)) + "." + Set(); // Putitin , print , reset
            if (!short_mode) output += std::string(" //Char ") + character + '\n'; // Comment
        }
        return output;
    }

    // Make sure to have room one space after target
    std::string DuplicateTo(unsigned int targetPos) {
        // TODO make it work even when one space after target isn't clean
        std::string output;
        unsigned int original_curr_pos = curr_pos;

        output = "["
                 + SetPos(targetPos)
                 + "+"
                 + SetPos(curr_pos + 1)
                 + "+"
                 + SetPos(original_curr_pos)
                 + "-]"
                 + SetPos(targetPos + 1)
                 + "["
                 + SetPos(original_curr_pos)
                 + "+"
                 + SetPos(targetPos + 1)
                 + "-]"
                  + SetPos(original_curr_pos);
        return output;
    }

    std::string EfficientText(const std::string& raw_text) {
        std::string output;

        const int ACII_CLOSENESS = 20;
        const int original_cursor_pos = curr_pos;

        std::vector<int> group_values;
        std::map<char, int> char_to_groupPos;

        for (char character : raw_text) {
            bool character_has_group = false;

            // Search for suitable startNum
            for (int group_value_index = 0; group_value_index < group_values.size(); ++group_value_index) {
                auto& group_value = group_values[group_value_index];
                if (abs(group_value - character) <= ACII_CLOSENESS) {
                    character_has_group = true;

                    // Assign character's value to i
                    char_to_groupPos[character] = group_value_index;
                    break; // Stop searching once a suitable group_value is found
                }
            }
            // If character didn't fit into any startNum, create a new one
            if (!character_has_group) {
                group_values.push_back(character); // Correctly initialize with the ASCII value
                char_to_groupPos[character] = group_values.size() - 1;
            }
        }

        // Initialize beginning numbers
        for (auto& group_value : group_values) {
            output += Add(group_value); // Initialize group value
            output += SetPos(curr_pos + 1); // Go right
            if (!short_mode) output += " //Initialize " + std::to_string(group_value) + "\n";
        }

        // Printing n stuf
        for (auto& character : raw_text) {
            int& group_pos = char_to_groupPos[character];
            int& group_value = group_values[group_pos];

            output += SetPos(group_pos + original_cursor_pos);
            int diffBetweenChars = character - group_value;


            group_value = group_value + diffBetweenChars; // Set group value to right num
            // Do the printing for what just happened
            if (diffBetweenChars > 0) {
                output += std::string(diffBetweenChars, '+');
                if (!short_mode) output += " //Add " + std::to_string(diffBetweenChars) + "\n";
            } else if (diffBetweenChars < 0) {
                output += std::string(-diffBetweenChars, '-');
                if (!short_mode) output += " //Subtract " + std::to_string(-diffBetweenChars) + "\n";
            }
            output += ".";
            if (!short_mode) output += " //Print\n";
        }

        // Delete initialized nums
        for (int i = curr_pos; i > original_cursor_pos - 1; --i) {
            output += SetPos(i); // Go back one
            output += Set(); // Set to 0
        }
        if (!short_mode) output += " //Reset initial values to zero\n";

        return output;
    }

    static std::string CleanupBF(std::string text, int wrap = 0) {
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

void ProcessCommand(const std::string& command, Brainfun& bf, bool debug = false) {
    if (debug) std::cout << "Processing command: " << command << std::endl;

    std::stringstream ss(command);
    std::string action;
    ss >> action; // Assuming the first word is the action/command
    if (action == "ADD") {
        int num;
        ss >> num; // Assuming an integer argument follows the command
        std::cout << bf.Add(num); // Example of executing a method based on the command
    } else if (action == "MULT") {
        int num1;
        ss >> num1;
        int num2;

        if (ss >> num2)
        {
            std::cout << bf.Mult(num1, num2);
        } else {
            std::cout << bf.Mult(num1);
        }
    } else if (action == "GO") {
        std::string pos;
        ss >> pos;
        if (pos == "RIGHT") {
            std::cout << bf.SetPos(bf.getPos() + 1);
        } else if (pos == "LEFT") {
            std::cout << bf.SetPos(bf.getPos() - 1);
        } else {
            std::cout << bf.SetPos(std::stoi(pos));
        }
    } else if (action == "SET") {
        int value;
        ss >> value;
        std::cout << bf.Set(value);
    } else if (action == "MOVE") {
        std::string pos;
        ss >> pos;
        if (pos == "RIGHT") {
            std::cout << bf.Move(bf.getPos() + 1);
        } else if (pos == "LEFT") {
            std::cout << bf.Move(bf.getPos() - 1);
        } else {
            std::cout << bf.Move(std::stoi(pos));
        }
    } else if (action == "CLONE") {
        std::string pos;
        ss >> pos;
        if (pos == "RIGHT") {
            std::cout << bf.DuplicateTo(bf.getPos() + 1);
        } else if (pos == "LEFT") {
            std::cout << bf.DuplicateTo(bf.getPos() - 1);
        } else {
            std::cout << bf.DuplicateTo(std::stoi(pos));
        }
    } else if (action == "TEXT") {
        std::string text;
        getline(ss >> std::ws, text);
        ss.ignore();
        std::cout << bf.EfficientText(text);
    } else if (action == "__") {
        std::string text;
        getline(ss >> std::ws, text);
        std::cout << text;
    }
    else if (action != "//"){
        std::cerr << "Unknown command: " << action << std::endl;
    }
}


void ExecuteFromFile(const std::string& filename, Brainfun& bf, bool debug = false) {
    std::ifstream file(filename);
    if (!file) {
        std::cerr << "Failed to open file: " << filename << std::endl;
        return;
    }

    std::string line;
    while (getline(file, line)) {
        if (debug) std::cout << "Read line: " << line << std::endl; // Debugging output
        ProcessCommand(line, bf, debug);
    }
}


int main() {
    Brainfun bf;

    ExecuteFromFile("../code.txt", bf, false);

    return 0;
}

// TODO have efficient_text have custom amount to skip forward instead of just going one
// TODO way of doing loops
// TODO have it take out <> or ><