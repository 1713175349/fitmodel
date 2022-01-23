#include"parameters_node.hpp"

parameters_node::parameters_node(/* args */)
{
}

parameters_node::parameters_node(const parameters_node &pn )
{
    name = pn.name;
    value = pn.value;
    max = pn.max;
    min = pn.min;
    for (int i = 0; i < pn.children.size(); i++)
    {
        children.push_back(new parameters_node(*pn.children[i]));
    }
    command = pn.command;
}

parameters_node::~parameters_node()
{
    // delete children
    for(auto &i:children){
        delete i;
    }
}
