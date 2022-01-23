#include "lammps_model.hpp"
#include"input.h"
#include"variable.h"
#include<cstring>
#include<fstream>
#include"para_fitness.hpp"
lammps_model::lammps_model(YAML::Node cfgs1)
{
    cfgs = cfgs1;
    init();
}

/**
 * @brief init lammps_model
 * 
 */
void lammps_model::init(){
    char name[]="lammps";
    char *nn=new char[strlen(name)+1];
    strcpy(nn,name);
    lmp=new LAMMPS_NS::LAMMPS(1,&nn,MPI_COMM_WORLD);
    lmp->screen=fopen("/dev/null","w");
    //lmp->logfile=fopen("/dev/null","w");
    lmp->input->file(cfgs["prein"].as<std::string>().c_str());
}

std::vector<double> lammps_model::compute(std::string command){
    std::vector<double> results;
    char *result = lmp->input->variable->retrieve(command.c_str()); // 计算变量的值
    double result1 = atof(result);
    return std::vector<double>{result1};
}

double lammps_model::fitness(){
    std::vector<double> results;
    for (int i = 0; i < cfgs["result"].size(); i++)
    {
        results.push_back(compute(cfgs["result"][i]["command"].as<std::string>())[0]-cfgs["result"][i]["value"].as<double>());
        results[i] = results[i]*cfgs["result"][i]["weight"].as<double>();
    }
    double fitnn = 0;
    for (int i = 0; i < results.size(); i++)
    {
        fitnn += results[i] * results[i];
    }

    return fitnn;
}

void lammps_model::forwardparas(){


    if (cfgs["calculatein"].IsDefined())
    {
        lmp->input->one("clear");
        lmp->input->file(cfgs["prein"].as<std::string>().c_str());
        // delete lmp;
        // init();
    }
    string modelstring = parafitness->cfgs["command_model"].as<string>();
    if (modelstring=="command"){
        for(int i=0;i<parafitness->parasdata.size();i++){
            parameters_node *para=&parafitness->parasdata[i];
            std::string command=para->command;
            for(int j=0;j<para->value.size();j++){
                std::string replaces=+"{"+std::to_string(j)+"}";
                command.replace(command.find(replaces),replaces.size(),std::to_string(para->value[j]));
            }
            lmp->input->one(command);
        }
    }
    else if (modelstring=="sw")
    {
        string swfilename = parafitness->cfgs["sw_filename"].as<string>();
        std::fstream fp(swfilename,std::ios::out);
        for(int i=0;i<parafitness->parasdata.size();i++){
            parameters_node *para=&parafitness->parasdata[i];
            std::string command=para->command;
            for(int j=0;j<para->value.size();j++){
                std::string replaces=+"{"+std::to_string(j)+"}";
                command.replace(command.find(replaces),replaces.size(),std::to_string(para->value[j]));
            }
            fp<<command<<"\n";
        }
        fp.close();
        std::string command=parafitness->cfgs["sw_command"].as<string>();
        command.replace(command.find("{swf}"),5,swfilename);
        lmp->input->one(command);
    }
    
    if (not cfgs["calculatein"].IsDefined())
    {
        lmp->input->one("run 0");
    }else
    {
        lmp->input->file(cfgs["calculatein"].as<std::string>().c_str());
    }


}

