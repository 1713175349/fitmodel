#pragma once

#include<yaml-cpp/yaml.h>


namespace YAML {
    template <typename T>
    struct convert<std::vector<T>>{
        static Node encode(const std::vector<T>& rhs){
            Node node;
            for(auto& i : rhs){
                node.push_back(i);
            }
            return node;
        }
        static bool decode(const Node& node, std::vector<T>& rhs){
            if(!node.IsSequence()){
                return false;
            }
            rhs.clear();
            for (int i=0;i<node.size();i++){
                rhs.push_back(node[i].as<T>());
            }
            return true;
        }
    };
}
