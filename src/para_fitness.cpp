#include "para_fitness.hpp"
#include "lammps_model.hpp"
#include<sstream>

para_fitness::para_fitness(std::string cfgs_path)
{
    cfgs = YAML::LoadFile(cfgs_path);
    init();
}

para_fitness::~para_fitness()
{
    for(auto &i:sys)
    {
        delete i;
    }
}

void para_fitness::init()
{
    generator.seed(std::random_device{}());
    
    // 初始化参数部分
    for (int i = 0; i < cfgs["parameterset"].size(); i++)
    {
        parasdata.push_back(parameters_node());
        parasdata[i].name = cfgs["parameterset"][i]["name"].as<std::string>();
        parasdata[i].command = cfgs["parameterset"][i]["command"]["lammps"]["model_command"].as<std::string>();
        parasdata[i].value = cfgs["parameterset"][i]["value"].as<vector<double>>();
        if (cfgs["parameterset"][i]["max"].IsDefined())
        {
            parasdata[i].max = cfgs["parameterset"][i]["max"].as<vector<double>>();
        }
        else
        {
            parasdata[i].max = parasdata[i].value;
            for (int j = 0; j < parasdata[i].max.size(); j++)
            {
                parasdata[i].max[j] = parasdata[i].value[j] * 10;
            }
        }
        if (cfgs["parameterset"][i]["min"].IsDefined())
        {
            parasdata[i].min = cfgs["parameterset"][i]["min"].as<vector<double>>();
        }
        else
        {
            parasdata[i].min = parasdata[i].value;
            for (int j = 0; j < parasdata[i].min.size(); j++)
            {
                parasdata[i].min[j] = -parasdata[i].value[j] * 10;
            }
        }
        // TODO subpara
    }
    int now = 0;
    for (int i = 0; i < parasdata.size(); i++)
    {
        for (int j = 0; j < parasdata[i].value.size(); j++)
        {
            parameters_incfg.push_back(&parasdata[i].value[j]);
            parameters.push_back(parasdata[i].value[j]);
            parameters_max.push_back(parasdata[i].max[j]);
            parameters_min.push_back(parasdata[i].min[j]);
            now++;
        }
    }

    // 初始化系统部分
    for (int i = 0; i < cfgs["models"].size(); i++)
    {
        sys.push_back(new lammps_model(cfgs["models"][i]));
        (sys[i])->parafitness = this;
    }
    
}

void para_fitness::forwardparas()
{
    for (int i = 0; i < parameters.size(); i++)
    {
        *parameters_incfg[i] = parameters[i];
    }
    for (int i = 0; i < sys.size(); i++)
    {
        (sys[i])->forwardparas();
    }
}


double para_fitness::fitness(){
    forwardparas();
    double result=0;
    for(int i=0;i<sys.size();i++){
        result+=sys[i]->fitness();
    }
    return result;
}

vector<double> para_fitness::generate_parameters(){
    vector<double> paras=parameters;
    std::uniform_real_distribution<double> distribution(0,1);
    for(int i=0;i<paras.size();i++){
        paras[i]= parameters_min[i]+distribution(generator)*(parameters_max[i]-parameters_min[i]);
    }
    return paras;
}

std::string para_fitness::get_paras_str(){
    std::stringstream ss;
    for(int i=0;i<parasdata.size();i++){
        ss<<parasdata[i].name<<" :  \n    ";
        for(int j=0;j<parasdata[i].value.size();j++){
            ss<<parasdata[i].value[j]<<" ";
        }
        ss<<"\n";
    }
    return ss.str();
}