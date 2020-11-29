#ifndef PROJECT_H
#define PROJECT_H

#include <vector>
#include <algorithm>
#include <iostream>

class Project
{
  public:
    virtual void run() = 0;

    static char wait_for_valid_char(std::string message, std::vector<char> accepted_values)
    {
        while (true)
        {
            std::cout << message;

            std::string input;
            std::cin >> input;
            const char input_char = tolower(input.at(0));

            if(std::find(accepted_values.cbegin(), accepted_values.cend(), input_char) != accepted_values.cend()) {
                return input_char;
            }
        }
    }

    static int wait_for_number(std::string message)
    {
        while (true)
        {
            std::cout << message;

            std::string input;
            std::cin >> input;

            // set non_digit_present to true by checking all chars in the input string
            bool non_digit_present = false;
            for (const char &c : std::as_const(input))
            {
                if (!std::isdigit(c))
                {
                    non_digit_present = true;
                    break;
                }
            }
            // If there is a non digit present or the value is ridiculous, don't accept it
            if (!non_digit_present)
            {
                int value = std::stoi(input);
                if (value >= 0 && value < 99)
                    return value;
            }
        }
        return -1;
    }

    static bool yes_or_no_to_bool(char input_char)
    {
        if (input_char == 'y')
        {
            return true;
        }
        else if (input_char == 'n')
        {
            return false;
        }
        // Maybe this should be a std::optional
        return false;
    }

    static std::string bool_to_yes_or_no(bool input_bool)
    {
        if (input_bool)
        {
            return "Yes";
        }
        else
        {
            return "No";
        }
    }
};

#endif // PROJECT_H