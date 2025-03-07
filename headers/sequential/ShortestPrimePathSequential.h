#ifndef SHORTESTPRIMEPATHSEQUENTIAL_H
#define SHORTESTPRIMEPATHSEQUENTIAL_H
#include "../IPathBuilder.h"

class ShortestPrimePathSequential: public IPathBuilder {
public:
    void displayPath(const std::string &start, const std::string &end, Graph *graph);

private:
    bool isPrime(int n);
    struct State {
        std::string node;
        int weight;
        std::vector<std::string> path;
    };
    struct StateComparator;
};

#endif //SHORTESTPRIMEPATHSEQUENTIAL_H
