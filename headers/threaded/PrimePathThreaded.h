#ifndef PRIMEPATHTHREADED_H
#define PRIMEPATHTHREADED_H
#include "../IPathBuilder.h"

class PrimePathThreaded: public IPathBuilder {
public:
    void displayPath(const std::string &start, const std::string &end, Graph* graph);
};

#endif //PRIMEPATHTHREADED_H
