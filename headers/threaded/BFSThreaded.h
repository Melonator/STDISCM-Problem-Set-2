#ifndef BFSTHREADED_H
#define BFSTHREADED_H
#include "../IPathBuilder.h"

class BFSThreaded: public IPathBuilder {
public:
    void displayPath(const std::string &start, const std::string &end, Graph* graph);
};

#endif //BFSTHREADED_H
