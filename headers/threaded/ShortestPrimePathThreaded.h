#ifndef SHORTESTPRIMEPATHTHREADED_H
#define SHORTESTPRIMEPATHTHREADED_H
#include "../IPathBuilder.h"

class ShortestPrimePathThreaded: public IPathBuilder {
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

#endif //SHORTESTPRIMEPATHTHREADED_H
