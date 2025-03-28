#ifndef AGENTSSIMULATOR_H
#define AGENTSSIMULATOR_H
#include "Graph.h"

class AgentsSimulator {
public:
    explicit AgentsSimulator(Graph* graph);
    void RunSimulation();

private:
    void Explore();
    Graph* graph;
};

#endif
