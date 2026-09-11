#include "model.h"
#include<fstream>
#include<string>
#include<sstream>
#include<iostream>

Model::Model(const char* filename){
    std::ifstream in(filename); //用in打开文件
    if(in.fail()){
        std::cerr<<"file opened in error!"<<std::endl;
        return;
    }

    std::string line;
    while (!in.eof()){
        std::getline(in, line); //如果没有到文件结尾EOF，循环读取每一行

        std::istringstream iss(line); //改用iss处理行
        std::string trash; //吞掉首个字符v/f

        if(!line.compare(0, 2, "v ")){ //如果匹配会返回0
            iss >> trash;

            Vector3f v;
            iss >> v.x >> v.y >> v.z;
            vertices.push_back(v);
        }
        else if (!line.compare(0, 2, "f ")){
            iss >> trash;

            char trash2; //吞掉斜杠
            std::vector<int> face;
            int v, vt, vn;

            while(iss >> v >> trash2 >> vt >> trash2 >> vn){ //若f行仍有数据组未读取完毕
                face.push_back(v - 1); //注意f保存的序号从1开始，转换为数组要-1
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