#include <iostream>
#include <algorithm>
#include <map>

#include "teamoptimizer.h"

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
            if (value > 0 && value < 99)
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

TeamOptimizer::TeamOptimizer()
{

}

void TeamOptimizer::run()
{
    std::string message = "Select a menu item:\n \
        [a] Add candidate to screening list\n \
        [l] List all candidates and their information\n \
        [v] View optimal candidates for each role\n \
        [e] Exit\n";

    std::vector<char> accepted_values = {'a', 'l', 'v', 'e'};

    while (true)
    {
        char input_char = wait_for_valid_char(message, accepted_values);
        if (input_char == 'a')
        {
            add_candidate();
        }
        else if (input_char == 'l')
        {
            list_candidates();
        }
        else if (input_char == 'v')
        {
            view_candidates();
        }
        else if (input_char == 'e')
        {
            break;
        }
    }
}

// I went for an indie game design team as my team to optimize
void TeamOptimizer::add_candidate()
{
    Candidate candidate;
    std::cout << "Enter candidate name:\n";
    std::cin >> candidate.name;
    candidate.years_programming = wait_for_number("How many years programming experience does the candidate have?\n");
    candidate.years_designing = wait_for_number("How many years programming experience does the candidate have?\n");
    candidate.years_leading = wait_for_number("How many years programming experience does the candidate have?\n");

    candidate_list.push_back(candidate);
}

void TeamOptimizer::list_candidates()
{
    for (const auto &candidate : std::as_const(candidate_list))
    {
        std::cout << "Candidate name: " << candidate.name << '\n'
        << "Programming experience: " << candidate.years_programming << " years\n"
        << "Design experience: " << candidate.years_designing << " years\n"
        << "Leadership experience: " << candidate.years_leading << " years\n"
        << "---\n";
    }
}

void TeamOptimizer::view_candidates()
{
    std::map<std::string, std::vector<Candidate>> map;
    map.insert({"Playtesters", {}});
    map.insert({"Leaders", {}});
    map.insert({"Designers", {}});
    map.insert({"Programmers", {}});

    for (const auto &candidate : std::as_const(candidate_list))
    {
        if (candidate.years_programming)
        {
            map.at("Programmers").push_back(candidate.name);
        }
        if (candidate.years_designing)
        {
            map.at("Designers").push_back(candidate.name);
        }
        if (candidate.years_leading)
        {
            map.at("Leaders").push_back(candidate.name);
        }

        if (!candidate.is_leader && !candidate.is_artist && !candidate.is_programmer)
        {
            map.at("Playtesters").push_back(candidate.name);
        }
    }

    for (const auto &[list_name, list] : std::as_const(map))
    {
        std::cout << list_name << ":\n";
        for (const auto &candidate_name : list)
        {
            std::cout << candidate_name << '\n';
        }
        std::cout << "---\n";
    }
}