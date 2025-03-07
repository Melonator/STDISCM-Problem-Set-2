#ifndef IPATHBUILDER_H
#define IPATHBUILDER_H
#include <string>

#include "Graph.h"

class IPathBuilder {
public:
    virtual void displayPath(const std::string& start, const std::string& end, Graph* graph);
};

#endif //IPATHBUILDER_H
