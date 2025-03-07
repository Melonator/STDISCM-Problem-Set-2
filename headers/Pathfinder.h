#ifndef PATHFINDER_H
#define PATHFINDER_H

#include <memory>

#include "Graph.h"
#include "../headers/IPathBuilder.h"

class Pathfinder {
public:
    explicit Pathfinder(Graph* graph);
    void setStrategy(IPathBuilder* strategy);
    void displayPath(const std::string& start, const std::string& end);


private:
    IPathBuilder* _path_builder = nullptr;
    Graph* graph = nullptr;
};

#endif //PATHFINDER_H
