#ifndef ASTARTHREADED_H
#define ASTARTHREADED_H
#include "../IPathBuilder.h"

class AStarThreaded: public IPathBuilder {
public:
    void displayPath(const std::string &start, const std::string &end);
};

#endif //ASTARTHREADED_H
