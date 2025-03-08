#ifndef SHORTESTPATHSEQUENTIAL_H
#define SHORTESTPATHSEQUENTIAL_H
#include "../IPathBuilder.h"

class ShortestPathSequential: public IPathBuilder {
public:
    void displayPath(const std::string &start, const std::string &end, Graph* graph);
};

#endif //SHORTESTPATHSEQUENTIAL_H
