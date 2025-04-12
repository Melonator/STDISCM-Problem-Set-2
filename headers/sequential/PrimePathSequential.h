#ifndef PRIMEPATHSEQUENTIAL_H
#define PRIMEPATHSEQUENTIAL_H
#include "../IPathBuilder.h"

class PrimePathSequential: public IPathBuilder {
public:
    std::vector<std::string> displayPath(const std::string &start, const std::string &end, Graph *graph);
private:
    bool isPrime(size_t n);
    struct State {
        std::string node;
        size_t weight;
        std::vector<std::string> path;
    };
};



#endif //PRIMEPATHSEQUENTIAL_H
