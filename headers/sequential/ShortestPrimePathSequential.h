#ifndef SHORTESTPRIMEPATHSEQUENTIAL_H
#define SHORTESTPRIMEPATHSEQUENTIAL_H
#include "../IPathBuilder.h"

class ShortestPrimePathSequential: public IPathBuilder {
public:
    void displayPath(const std::string &start, const std::string &end) override;
};

#endif //SHORTESTPRIMEPATHSEQUENTIAL_H
