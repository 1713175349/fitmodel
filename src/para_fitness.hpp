#pragma once

#include "model.hpp"
#include<vector>
#include<string>
#include<map>
#include<yaml-cpp/yaml.h>
#include<random>
#include"yaml-vector.hpp"
#include"parameters_node.hpp"
#include"lammps_model.hpp"


using std::vector;


/**
 * @brief 用于产生参数，计算适应度，与优化库进行交互
 * 
 */
class para_fitness
{
public:
    /* data */
    vector<lammps_model *> sys; 
    vector<double> parameters;

    vector<double*> parameters_incfg;//用于更新参数
    vector<double> parameters_max;
    vector<double> parameters_min;

    vector<parameters_node> parasdata;
    YAML::Node cfgs;//用于存储设置
    std::default_random_engine generator;
public:
    para_fitness(/* args */)=default;
    para_fitness(std::string cfgs_path);
    void init();
    void forwardparas();
    std::string get_paras_str();
    // virtual std::string decode() = 0;
    double fitness();
    vector<double> generate_parameters();
    double operator()(const vector<double> &ps){parameters = ps;return fitness();} // 利用输入的参数更新系统
    ~para_fitness();
};
