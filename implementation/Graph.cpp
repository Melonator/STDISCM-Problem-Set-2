#include "../headers/Graph.h"
#include <fstream>
#include <iostream>

#pragma region PSET2
Edge::Edge(const std::string &node, const size_t weight) {
    this->node = node;
    this->weight = weight;
}

Graph::Graph(const std::string &file_name) {
    parseGraph(file_name);
}

//TODO: Update parsing algorithm
// set numAgents private variable
// set the initialAgentsData private variable(i.e., x, y, startnode)
void Graph::parseGraph(const std::string &file_name) {
    std::ifstream File(file_name);

    std::string symbol;
    std::string node;
    std::string neighbor;
    size_t weight;
    if (!File.is_open()) {
        return;
    }

    std::cout << "File read!" << "\n";
    while (!File.eof()) {
        if (!File.good()) {
            break;
        }
        File >> symbol;
        if (symbol == "*") {
            File >> node;
            graph[node] = {};
        }
        else if (symbol == "-") {
            File >> node;
            File >> neighbor;
            File >> weight;

            graph[node].emplace_back(Edge(neighbor, weight));
        }
    }

    File.close();
    std::cout << "File done reading!\n\n";
}

std::vector<Edge> Graph::getNeighbors(const std::string &node) const {
    return graph.count(node) ? graph.at(node) : std::vector<Edge>{};
}

void Graph::displayEdges() {

}

void Graph::displayNodes() {

}

std::vector<std::string> Graph::getNodes() const {
    std::vector<std::string> nodes;
    for (const auto & it : graph) {
        nodes.emplace_back(it.first);
    }

    return nodes;
}

size_t Graph::getNumNodes() const{
   return graph.size();
}

bool Graph::hasEdge(const std::string &node1, const std::string &node2) const {
    if (!hasNode(node1)) {
        return false;
    }

    for (const auto edgeList = graph.at(node1); auto edge : edgeList) {
        if (edge.node == node2) {
            return true;
        }
    }

    return false;
}

bool Graph::hasNode(const std::string &node) const {
    try {
        graph.at(node);
        return true;
    } catch (std::exception& e) {
        return false;
    }
}

#pragma endregion

#pragma region PSET3
std::size_t Graph::getNumAgents() const {
    return numAgents;
}

std::vector<InitialAgentData> Graph::getInitialAgentsData() const {
    return initialAgentsData;
}

//TODO: new get neighbors that only has a specific weight for the edge
std::vector<Edge> Graph::getNeighbors(const std::string& node, size_t weight) const {

}

#pragma endregion





