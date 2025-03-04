#ifndef PRIMEPATHSEQUENTIAL_H
#define PRIMEPATHSEQUENTIAL_H
#include "../IPathBuilder.h"

class PrimePathSequential: public IPathBuilder {
public:
    void displayPath(const std::string &start, const std::string &end) override;
};

#endif //PRIMEPATHSEQUENTIAL_H
