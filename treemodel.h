#ifndef TREEMODEL_H
#define TREEMODEL_H

#include <string>
#include <vector>

#include "project.h"

class Node {
public:
    explicit Node(const std::string &_key, unsigned __int128 _val, Node *_parent = nullptr) : key(_key), val(_val), parent(_parent), child0(nullptr), child1(nullptr) {};

    std::string get_key() const { return key; };
    unsigned __int128 get_val() const { return val; }
    Node *&get_child0() { return child0; }
    Node *&get_child1() { return child1; }

private:
    Node *parent;
    std::string key;
    unsigned __int128 val; // __int128 is supported on gcc and clang 

    Node *child0;
    Node *child1;
};

class TreeModel : Project 
{
public:
    explicit TreeModel();

    void populate_nodes(int root_index);

    void run() override;

private:
    Node *root_node;

    // Masses from https://en.wikipedia.org/wiki/List_of_Solar_System_objects_by_size
    const std::vector<std::pair<std::string, unsigned __int128>> PLANET_INFO = {
        {"Sun", 1988500000e21},
        {"Mercury", 330.1e21},
        {"Venus", 4867.5e21},
        {"Earth", 5972.4e21},
        {"Mars", 641.7e21},
        {"Saturn", 568340e21},
        {"Jupiter", 1898200e21},
        {"Neptune", 102413e21},
        {"Uranus", 86813e21},
        {"Pluto", 13.03e21},
        {"Ceres", 0.938e21},
        {"Io", 89.32e21},
        {"Europa", 48e21},
        {"Moon", 73.42e21},
        {"Phobos", 10.659e15},
        {"162173 Ryugu", 450e9}
    };
};


#endif // TREEMODEL_H
