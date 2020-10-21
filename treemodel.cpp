#include "treemodel.h"

#include <iostream>
#include <random>

//To-do: destructors for pointers
//To-do: print __int128 :(

TreeModel::TreeModel()
{
   root_node = nullptr; 
}

static Node*& child0_or_child1(const unsigned __int128 &val, Node *candidate_node)
{
    if (val > candidate_node->get_val())
        return candidate_node->get_child1();
    else
        return candidate_node->get_child0();
}

static std::pair<Node*&, Node*> find_place_for_value(const unsigned __int128 &val, Node *candidate_node)
{
    Node *&child = child0_or_child1(val, candidate_node);

    if (!child)
        return {child, candidate_node};

    return find_place_for_value(val, child);
}

void TreeModel::delete_node(Node *node)
{
    if (!node)
        return;

    Node *new_node = nullptr;
    Node *sort_me_later = nullptr;

    if (node->get_child0() && node->get_child1())
    {
         new_node = node->get_child1();
         sort_me_later = node->get_child0();
         node->get_child0() = nullptr;
    }
    else if (node->get_child0() && !node->get_child1())
    {
        new_node = node->get_child0();
    }
    else if (node->get_child1() && !node->get_child0())
    {
        new_node = node->get_child1();
    }

    if (root_node == node)
    {
        root_node = new_node;
    }
    else if (node->get_parent()->get_child0() == node)
    {
        node->get_parent()->get_child0() = new_node;
    }
    else if (node->get_parent()->get_child1() == node)
    {
        node->get_parent()->get_child1() = new_node;
    }

    delete node;

    if (sort_me_later)
       insert_node(sort_me_later);
}


void TreeModel::insert_node(Node *node)
{
    if (!node)
        return;

    auto [new_pos, new_parent] = find_place_for_value(node->get_val(), root_node);
    node->get_parent() = new_parent;
    new_pos = node;
}

void TreeModel::populate_nodes(int root_index)
{
    root_node = new Node(PLANET_INFO[root_index].first, PLANET_INFO[root_index].second, nullptr);

    for (int i = 0; i < PLANET_INFO.size(); i++)
    {
        if (i == root_index)
            continue;

        auto *new_node = new Node(PLANET_INFO[i].first, PLANET_INFO[i].second);
        insert_node(new_node);
    }
}

static void print_recursive_child_nodes(Node *node, int indentation = 0, bool is_child0 = false)
{
    for (int i = 0; i < indentation; i++)
        std::cout << "    ";
   
    std::cout << (is_child0 ? "├──" : "└──");

    std::cout << node->get_key() << std::endl;

    if (node->get_child0())
    {
        print_recursive_child_nodes(node->get_child0(), indentation + 1, true);
    }

    if (node->get_child1())
    {
        print_recursive_child_nodes(node->get_child1(), indentation + 1);
    }
}

void TreeModel::run()
{
    std::random_device random_device;
    std::mt19937 generator(random_device());
    std::uniform_int_distribution<int> distribution(0, PLANET_INFO.size());

    int random_index = distribution(generator);
    std::cout << "root index " << random_index << std::endl;

    populate_nodes(0);

    delete_node(root_node);

    print_recursive_child_nodes(root_node);
}
