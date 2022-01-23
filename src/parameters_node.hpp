#pragma once
#include<string>
#include<vector>

using std::string;
using std::vector;
class parameters_node
{
public:
    /* data */
    string name;
    vector<double> value;
    vector<double> max;
    vector<double> min;
    vector<parameters_node *> children;
    string command;
public:
    parameters_node(/* args */);
    parameters_node(const parameters_node &);
    ~parameters_node();
};
