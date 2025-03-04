#include "../headers/Graph.h"

#include <fstream>
#include <iostream>

Edge::Edge(const std::string &node, const int weight) {
    this->node = node;
    this->weight = weight;
}

Graph::Graph(const std::string &file_name) {
    parseGraph(file_name);
}

void Graph::parseGraph(const std::string &file_name) {
    std::ifstream File(file_name);

    std::string symbol;
    std::string node;
    std::string neighbor;
    int weight;
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
    return graph.at(node);
}

void Graph::displayEdges() {

}

void Graph::displayNodes() {

}

bool Graph::hasEdge(const std::string &node1, const std::string &node2) {

}

bool Graph::hasNode(const std::string &node) {

}






