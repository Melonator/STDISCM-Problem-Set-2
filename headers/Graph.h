//
// Created by MY PC on 3/4/2025.
//

#ifndef GRAPH_H
#define GRAPH_H
#include <string>
#include <unordered_map>
#include <vector>

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
    bool hasNode(const std::string& node) const;
    bool hasEdge(const std::string& node1, const std::string& node2) const;
    std::vector<Edge> getNeighbors(const std::string& node) const;

protected:
    std::unordered_map<std::string, std::vector<Edge>> graph;

private:
    void parseGraph(const std::string& file_name);
};

#endif //GRAPH_H
