#ifndef ASTARSEQUENTIAL_H
#define ASTARSEQUENTIAL_H
#include "../IPathBuilder.h"

class AStarSequential: public IPathBuilder {
public:
    void displayPath(const std::string &start, const std::string &end);
};

#endif //ASTARSEQUENTIAL_H
