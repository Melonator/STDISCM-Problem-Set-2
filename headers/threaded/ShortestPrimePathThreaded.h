#ifndef SHORTESTPRIMEPATHTHREADED_H
#define SHORTESTPRIMEPATHTHREADED_H
#include "../IPathBuilder.h"
#include <string>
#include <vector>

class ShortestPrimePathThreaded: public IPathBuilder {
public:
    void displayPath(const std::string &start, const std::string &end, Graph* graph) override;
    struct State {
        std::string node;
        size_t weight;
        std::vector<std::string> path;
    };
private:
    static bool isPrime(size_t n);

};
#endif //SHORTESTPRIMEPATHTHREADED_H