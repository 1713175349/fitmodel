#include<pagmo/pagmo.hpp>
#include<pagmo/types.hpp>
#include<iostream>
#include"lammps_model.hpp"
#include"para_fitness.hpp"
#include<yaml-cpp/yaml.h>
#include<nlopt.hpp>
#include<mpi.h>
std::ostream &operator<<(std::ostream &os, const std::vector<double> &v)
{
    for (auto &i:v)
    {
        os << i << " ";
    }
    return os;
}
using namespace pagmo;
struct problem_v0
{
    para_fitness *pf;
    problem_v0()=default;
    problem_v0(para_fitness *pf_):pf(pf_){}
    vector_double fitness(const vector_double &x) const
    {
        return {(*pf)(x)};
    }

    std::pair<vector_double,vector_double> get_bounds() const
    {
        return {pf->parameters_min,pf->parameters_max};
    }
};

// struct problem_v0 {

//     // Implementation of the objective function.

//     vector_double fitness(const vector_double &dv) const
//     {
//         return {dv[0] * dv[3] * (dv[0] + dv[1] + dv[2]) + dv[2]};
//     }

//     // Implementation of the box bounds.
//     std::pair<vector_double, vector_double> get_bounds() const
//     {
//         return {{1., 1., 1., 1.}, {5., 5., 5., 5.}};
//     }
// };

int main(int arg,char **argv){
    MPI_Init(&arg,&argv);

    para_fitness pf("sw.yaml");
    problem_v0 pp{&pf};
    problem p{pp};
    std::cout<<p.get_lb()<<std::endl;
    std::cout<<p.get_ub()<<std::endl;
    population pop{p,100};
    
    pso user_algo1{100u, 0.79, 2., 2., 0.1, 4u, 4u, 4u, false, 23u};
    
    user_algo1.set_verbosity(1u);
    pop = user_algo1.evolve(pop);

    std::cout<<pop.best_idx()<<std::endl;
    std::cout<<pop.champion_f()<<std::endl;
    std::cout<<pop.champion_x()<<std::endl;
    MPI_Finalize();

    return 0;
}