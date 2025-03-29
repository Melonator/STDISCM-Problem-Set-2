#ifndef AGENTSSIMULATOR_H
#define AGENTSSIMULATOR_H
#include "Graph.h"

class AgentsSimulator {
public:
    explicit AgentsSimulator(Graph* graph);
    void RunSimulation();

private:
    std::unordered_map<std::string, std::mutex*> graphMutexes;
    void Explore(const InitialAgentData &initial_agent_data) const;
    Graph* graph;
};

#endif
