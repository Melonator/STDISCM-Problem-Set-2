#ifndef SHORTESTPATHTHREADED_H
#define SHORTESTPATHTHREADED_H
#include "../IPathBuilder.h"

class ShortestPathThreaded: public IPathBuilder {
public:
    void displayPath(const std::string &start, const std::string &end, Graph* graph);
};

#endif //SHORTESTPATHTHREADED_H
