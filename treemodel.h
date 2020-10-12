#ifndef TREEMODEL_H
#define TREEMODEL_H

#include <string>

#include "project.h"

class Node {
public:
    explicit Node(const std::string &_key, unsigned __int128 _val, Node *_parent = nullptr) : key(_key), val(_val), parent(_parent) {};
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

    void run() override;

private:
    Node *root_node;
};


#endif // TREEMODEL_H
