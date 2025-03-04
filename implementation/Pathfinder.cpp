#include "../headers/Pathfinder.h"
#include <iostream>

void Pathfinder::setStrategy(IPathBuilder* path_builder){
    _path_builder =  path_builder;
}

void Pathfinder::displayPath(const std::string& start,const std::string& end) {
    if (_path_builder) {
        _path_builder->displayPath(start, end);
    } else {
        std::cerr << "Error: _path_builder is NULL!" << std::endl;
    }
}