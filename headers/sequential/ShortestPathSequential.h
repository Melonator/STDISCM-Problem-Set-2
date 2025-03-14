#ifndef SHORTESTPATHSEQUENTIAL_H
#define SHORTESTPATHSEQUENTIAL_H
#include "../IPathBuilder.h"

class ShortestPathSequential: public IPathBuilder {
public:
    void displayPath(const std::string &start, const std::string &end, Graph* graph);
private:
    struct State {
        std::string node;              // Current node label.
        size_t weight;                    // Cumulative weight from the start node.
        std::vector<std::string> path; // The path taken so far.
    };
    struct StateComparator {
        bool operator()(const State &a, const State &b) const {
            return a.weight > b.weight;
        }
    };
};

#endif //SHORTESTPATHSEQUENTIAL_H
