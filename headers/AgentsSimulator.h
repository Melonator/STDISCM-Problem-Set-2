#ifndef AGENTSSIMULATOR_H
#define AGENTSSIMULATOR_H
#include <sstream>

#include "Graph.h"
#include "LogEntry.h"
#include "NodeMonitor.h"
#include "Pathfinder.h"

class AgentsSimulator {
public:
    explicit AgentsSimulator(Graph* graph);
    void runSimulation();
private:
    void explore(int agent_id, InitialAgentData agent_data, std::vector<LogEntry>& log_entries) const;
    std::vector<std::string> findPath(std::string start, std::string end, size_t weight, int agent_id);
    Graph* graph;
    std::vector<std::vector<std::string>> agentPaths;
    std::unordered_map<std::string, NodeMonitor*> nodeMonitors;
};

#endif
