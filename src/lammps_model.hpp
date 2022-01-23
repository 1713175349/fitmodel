#pragma once
#include "model.hpp"
#include "lammps.h"
#include<mpi.h>
class lammps_model:public model
{
public:
    /* data */
    LAMMPS_NS::LAMMPS *lmp;
    class para_fitness *parafitness;
public:
    lammps_model(/* args */)=default;
    lammps_model(YAML::Node cfgs1);
    void init();
    void forwardparas();
    std::vector<double> compute(std::string command);
    double fitness();
    // ~lammps_model();
};
