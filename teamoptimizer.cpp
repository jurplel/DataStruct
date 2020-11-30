#include <iostream>
#include <algorithm>
#include <vector>

#include "teamoptimizer.h"

TeamOptimizer::TeamOptimizer()
{

}

void TeamOptimizer::run()
{
    std::string message = "Select a menu item:\n \
        [a] Add candidate to screening list\n \
        [l] List all candidates and their information\n \
        [m] Modify candidate information\n \
        [r] Remove candidate\n \
        [s] Select role for candidate\n \
        [v] View optimal candidates for each role\n \
        [e] Exit team optimizer\n";

    std::vector<char> accepted_values = {'a', 'l', 'm', 'r', 's', 'v', 'e'};

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
        else if (input_char == 's' || input_char == 'm' || input_char == 'r')
        {
            select_candidates(input_char);
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
    std::string candidate_name;
    std::cout << "Enter candidate name:\n";
    std::cin >> candidate_name;
    set_candidate_info(candidate_name, candidate);
    candidate_list.insert({candidate_name, candidate});
}

void TeamOptimizer::set_candidate_info(std::string candidate_name, Candidate &candidate)
{
    candidate.years_programming = wait_for_number("How many years programming experience does the candidate have?\n");
    candidate.years_designing = wait_for_number("How many years design experience does the candidate have?\n");
    candidate.years_leading = wait_for_number("How many years leadership experience does the candidate have?\n");

}

void TeamOptimizer::list_candidates()
{
    for (const auto &[candidate_name, candidate] : std::as_const(candidate_list))
    {
        std::cout << "Candidate name: " << candidate_name << '\n'
        << "Programming experience: " << candidate.years_programming << " years\n"
        << "Design experience: " << candidate.years_designing << " years\n"
        << "Leadership experience: " << candidate.years_leading << " years\n"
        << "---\n";
    }
}

void TeamOptimizer::select_candidates(const char &input_mode)
{
    std::cout << "Candidate name (case-sensitive): ";
    std::string input;
    std::cin >> input;
    try {
        auto &candidate = candidate_list.at(input);
        switch (input_mode) {
            case 's': {
                std::string message = "Select role: [p]rogrammer, [d]esigner, [l]eader, or play[t]ester\n";
                std::vector<char> accepted_values = {'p', 'd', 'l', 't'};
                char input_char = wait_for_valid_char(message, accepted_values);
                candidate.appointed_role = input_char;
                break;
            }
            case 'm': {
                set_candidate_info(input, candidate);
                break;
            }
            case 'r': {
                candidate_list.erase(input);
                break;   
            }
        }
    }
    catch (...)
    {
        std::cout << "Candidate \"" << input << "\" not found\n";
    }
}

void TeamOptimizer::view_candidates()
{
    std::map<std::string, std::vector<std::string>> map;
    //  = {{"Programmers", {}}, {"Designers", {}}, {"Leaders", {}}, {"Playtesters", {}}};
    map.insert({"Playtesters", {}});
    map.insert({"Leaders", {}});
    map.insert({"Designers", {}});
    map.insert({"Programmers", {}});

    for (const auto &[candidate_name, candidate] : std::as_const(candidate_list))
    {
        if (candidate.years_programming > 0 || candidate.appointed_role == 'p')
        {
            map.at("Programmers").push_back(candidate_name);
        }
        if (candidate.years_designing > 0 || candidate.appointed_role == 'd')
        {
            map.at("Designers").push_back(candidate_name);
        }
        if (candidate.years_leading > 0 || candidate.appointed_role == 'l')
        {
            map.at("Leaders").push_back(candidate_name);
        }

        if ((!candidate.years_leading && !candidate.years_designing && !candidate.years_programming) || candidate.appointed_role == 't')
        {
            map.at("Playtesters").push_back(candidate_name);
        }
    }

    for (const auto &[list_name, list] : std::as_const(map))
    {
        std::cout << list_name << ":\n";
        for (const auto &candidate_name : as_const(list))
        {
            std::cout << candidate_name << ' ';

            Candidate candidate = candidate_list.at(candidate_name);
            if (list_name == "Programmers")
            {
                std::cout << '(' << candidate.years_programming << " years experience)";
                if (candidate.appointed_role == 'p')
                {
                    std::cout << " \032(Appointed)";
                }
            } 
            else if (list_name == "Designers")
            {
                std::cout << '(' << candidate.years_designing << " years experience)";
                if (candidate.appointed_role == 'd')
                {
                    std::cout << " \032(Appointed)";
                }
            }
            else if (list_name == "Leaders")
            {
                std::cout << '(' << candidate.years_leading << " years experience)";
                if (candidate.appointed_role == 'l')
                {
                    std::cout << " \032(Appointed)";
                }
            }
            else if (list_name == "Playtesters" && candidate.appointed_role == 't')
            {
                std::cout << " \032(Appointed)";
            }
        }
        std::cout << "\n---\n";
    }
}