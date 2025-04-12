#ifndef BFSSEQUENTIAL_H
#define BFSSEQUENTIAL_H
#include "../IPathBuilder.h"

class BFSSequential: public IPathBuilder {
public:
    std::vector<std::string> displayPath(const std::string &start, const std::string &end, Graph* graph);
};

#endif //BFSSEQUENTIAL_H
