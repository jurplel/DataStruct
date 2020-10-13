#include "treemodel.h"

#include <iostream>
#include <random>

//To-do: destructors for pointers

TreeModel::TreeModel()
{
   root_node = nullptr; 
}

static std::pair<Node*&, Node*> find_place_for_value(const unsigned __int128 &val, Node *candidate_node)
{
    Node *&place = candidate_node->get_child0();

    if (val > candidate_node->get_val())
        place = candidate_node->get_child1();

    if (!place)
        return {place, candidate_node};

    return find_place_for_value(val, place);
}

void TreeModel::populate_nodes(int root_index)
{
    root_node = new Node(PLANET_INFO[root_index].first, PLANET_INFO[root_index].second, nullptr);

    for (int i = 0; i < PLANET_INFO.size(); i++)
    {
        if (i == root_index)
            continue;

        auto [new_pos, new_parent] = find_place_for_value(PLANET_INFO[i].second, root_node);
        new_pos = new Node(PLANET_INFO[i].first, PLANET_INFO[i].second, new_parent);
    }
}

static void print_recursive_child_nodes(Node *node, int indentation = 0)
{
    for (int i = 0; i < indentation; i++)
        std::cout << '\t';
    
    std::cout << node->get_key() << std::endl;

//    if (node->get_child0())
//        print_recursive_child_nodes(node->get_child0(), indentation + 1);
    
//    if (node->get_child1())
//        print_recursive_child_nodes(node->get_child1(), indentation + 1);
}

void TreeModel::run()
{
    std::random_device random_device;
    std::mt19937 generator(random_device());
    std::uniform_int_distribution<int> distribution(0, PLANET_INFO.size());

    int random_index = distribution(generator);
    std::cout << "root index " << random_index << std::endl;

    //populate_nodes(random_index);
    populate_nodes(15);
    print_recursive_child_nodes(root_node);
}
