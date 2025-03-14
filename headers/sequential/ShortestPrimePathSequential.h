#ifndef SHORTESTPRIMEPATHSEQUENTIAL_H
#define SHORTESTPRIMEPATHSEQUENTIAL_H
#include "../IPathBuilder.h"

class ShortestPrimePathSequential: public IPathBuilder {
public:
    void displayPath(const std::string &start, const std::string &end, Graph *graph);

private:
    bool isPrime(size_t n);
    struct State {
        std::string node;
        size_t weight;
        std::vector<std::string> path;
    };
    struct StateComparator;
};

#endif //SHORTESTPRIMEPATHSEQUENTIAL_H
