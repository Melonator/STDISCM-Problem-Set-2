#ifndef SHORTESTPRIMEPATHTHREADED_H
#define SHORTESTPRIMEPATHTHREADED_H
#include "../IPathBuilder.h"

class ShortestPrimePathThreaded: public IPathBuilder {
public:
    void displayPath(const std::string &start, const std::string &end) override;
};

#endif //SHORTESTPRIMEPATHTHREADED_H
