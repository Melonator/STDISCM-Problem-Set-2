#ifndef PATHFINDER_H
#define PATHFINDER_H

#include <memory>

#include "../headers/IPathBuilder.h"

class Pathfinder {
public:
    Pathfinder(){};
    void setStrategy(IPathBuilder* strategy);
    void displayPath(const std::string& start, const std::string& end);


private:
    IPathBuilder* _path_builder = nullptr;
};

#endif //PATHFINDER_H
