#include "treemodel.h"

TreeModel::TreeModel()
{
   root_node = nullptr; 
}

void TreeModel::run()
{
    root_node = new Node("io", 89.32e21, nullptr);

}
