#include<iostream>
#include"lammps_model.hpp"
#include"para_fitness.hpp"
#include<yaml-cpp/yaml.h>
#include<nlopt.hpp>
#include<mpi.h>



double myfunc(const std::vector<double> &x, std::vector<double> &grad, void *my_func_data)
{
    para_fitness *pf = (para_fitness *)my_func_data;
    //std::cout<<"call myfunc"<<std::endl;
    return (*pf)(x);
}

std::ostream &operator<<(std::ostream &os, const std::vector<double> &v)
{
    for (auto &i:v)
    {
        os << i << " ";
    }
    return os;
}

int main(int argc,char **argv)
{
    MPI_Init(&argc,&argv);

    // std::string cfgs_path = "../try.yaml";
    // YAML::Node cfgs = YAML::LoadFile(cfgs_path);
    // lammps_model lm(cfgs["models"][0]);
    // std::cout<<"fdjk"<<std::endl;
    // std::cout<<lm.compute("energy")[0]<<std::endl;
    // std::cout << "Hello World!" << std::endl;
    para_fitness pf("sw.yaml");
    std::cout<<pf.fitness()<<std::endl;
    std::vector<double> ps = pf.generate_parameters();
    // std::cout<<ps[0]<<std::endl;
    // std::cout<<ps[1]<<std::endl;
    // std::cout<<pf(ps)<<std::endl;


    nlopt::opt opt1(nlopt::GN_DIRECT,pf.parameters.size());
    opt1.set_lower_bounds(pf.parameters_min);
    opt1.set_upper_bounds(pf.parameters_max);

    opt1.set_min_objective(myfunc,&pf);

    opt1.set_xtol_rel(1e-4);

    double minf;
    vector<double> xx=pf.parameters;
    try{
        nlopt::result result = opt1.optimize(xx,minf);
        std::cout<<"result: "<<minf<<std::endl;
        std::cout<<"x: "<<xx<<std::endl;
    }
    catch(std::exception &e){
        std::cout<<"nlopt failed: "<<e.what()<<std::endl;
    }
    pf(xx);
    std::cout<<"parameters: "<<pf.get_paras_str()<<std::endl;
    MPI_Finalize();
    return 0;
}