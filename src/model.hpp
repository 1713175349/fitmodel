#pragma once
#include<string>
#include<vector>
#include<yaml-cpp/yaml.h>

class model
{
public:
    /* data */
    std::string model_type="lammps";
    YAML::Node cfgs;
    class para_fitness *parafitness;
public:
    // model(/* args */)=default;
    virtual void init() = 0;
    // std::vector<double> results();
    virtual void forwardparas() = 0;
    virtual std::vector<double> compute(std::string command) = 0;
    virtual double fitness() = 0;
    // // virtual std::vector<double> result() = 0;
    // ~model();
};
