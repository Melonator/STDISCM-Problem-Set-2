#ifndef PRIMEPATHTHREADED_H
#define PRIMEPATHTHREADED_H
#include "../IPathBuilder.h"

class PrimePathThreaded: public IPathBuilder {
public:
    void displayPath(const std::string &start, const std::string &end, Graph* graph);
private:
    static bool isPrime(int n);
    struct State {
        std::string node;
        int weight;
        std::vector<std::string> path;
    };
};

#endif //PRIMEPATHTHREADED_H
