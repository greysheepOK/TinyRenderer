#include "model.h"
#include<fstream>
#include<string>
#include<sstream>
#include<iostream>

Model::Model(const char* filename){
    std::ifstream in(filename);
    if(in.fail()){
        std::cerr<<"file opened in error!"<<std::endl;
        return;
    }

    std::string line;
    while (!in.eof()){
        std::getline(in, line);

        std::istringstream iss(line);
        std::string trash;

        if(!line.compare(0, 2, "v ")){
            iss >> trash;

            Vector3f v;
            iss >> v.x >> v.y >> v.z;
            vertices.push_back(v);
        }
        else if (!line.compare(0, 2, "f ")){
            iss >> trash;

            char trash2;
            std::vector<int> face;
            int v, vt, vn;

            while(iss >> v >> trash2 >> vt >> trash2 >> vn){
                face.push_back(v - 1);
            }
            faces.push_back(face);
        }
    }

    std::cout << "model is loaded." << std::endl;
    std::cout << "numbers of vertices: " << vertices.size() << std::endl;
    std::cout << "numbers of faces: " << faces.size() << std::endl;
}

Model::~Model(){
    return;
}