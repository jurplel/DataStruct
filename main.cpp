#include <iostream>

#include "project.h"
#include "teamoptimizer.h"
#include "datamodel.h"
#include "treemodel.h"

void display_main_menu()
{
    std::string message = "Select a project:\n \
        [1] Team Optimizer (game dev team)\n \
        [2] Data Model (usb devices)\n \
        [3] Tree Model (planets)\n \
        [4] Search Trees (interstellar scenario/final)\n \
        [e] Exit program\n";

    std::vector<char> accepted_values = {'1', '2', '3', '4', 'e'};

    bool exit = false;
    while (!exit)
    {
        char input_char = Project::wait_for_valid_char(message, accepted_values);
        switch (input_char) {
            case '1': {
                TeamOptimizer team_optimizer;
                team_optimizer.run();
                break;
            }
            case '2': {
                DataModel data_model;
                data_model.run();
                break;
            }
            case '3': {
                TreeModel tree_model;
                tree_model.run();
                break;
            }
            case '4': {

                break;
            }
            case 'e': {
                exit = true;
                break;
            }
        }
    }
}

int main() 
{
    display_main_menu();
}