#include "../headers/Pathfinder.h"
#include <iostream>

Pathfinder::Pathfinder(Graph* graph) : graph(graph) {}
void Pathfinder::setStrategy(IPathBuilder* strategy){
    _path_builder = strategy;
}

std::vector<std::string> Pathfinder::displayPath(const std::string& start,const std::string& end, Graph* graph) {
    if (_path_builder) {
        return _path_builder->displayPath(start, end, graph);
    } else {
        std::cerr << "Error: _path_builder is NULL!" << std::endl;
    }
}