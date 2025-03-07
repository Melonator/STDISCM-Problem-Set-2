#ifndef PRIMEPATHSEQUENTIAL_H
#define PRIMEPATHSEQUENTIAL_H
#include "../IPathBuilder.h"

class PrimePathSequential: public IPathBuilder {
public:
    void displayPath(const std::string &start, const std::string &end);
};

#endif //PRIMEPATHSEQUENTIAL_H
