//
// Created by MY PC on 3/4/2025.
//

#ifndef GRAPH_H
#define GRAPH_H
#include <mutex>
#include <string>
#include <unordered_map>
#include <vector>

#include "InitialAgentData.h"

struct Edge {
    std::string node;
    size_t weight;
    Edge(const std::string& node, const size_t weight);
};

class Graph {
public:
    explicit Graph(const std::string& file_name);
    void displayNodes();
    void displayEdges();
    size_t getNumNodes() const;
    std::vector<std::string> getNodes() const;
    bool hasNode(const std::string& node) const;
    bool hasEdge(const std::string& node1, const std::string& node2) const;
    std::vector<Edge> getNeighbors(const std::string& node) const;
    std::vector<Edge> getNeighbors(const std::string& node, size_t weight) const;
    std::vector<InitialAgentData> getInitialAgentsData() const;
    std::size_t getNumAgents() const;

protected:
    std::unordered_map<std::string, std::vector<Edge>> graph;

private:
    int numAgents;
    std::vector<InitialAgentData> initialAgentsData;
    void parseGraph(const std::string& file_name);
};

#endif //GRAPH_H
