#include "../headers/Graph.h"

#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>

#pragma region PSET2
Edge::Edge(const std::string &node, const size_t weight) {
    this->node = node;
    this->weight = weight;
}

Graph::Graph(const std::string &file_name) {
    parseGraph(file_name);
}

void Graph::parseGraph(const std::string& file_name)
{
    std::ifstream file(file_name);
    if (!file) {
        std::cerr << "Could not open " << file_name << '\n';
        return;
    }

    auto isAgent = [](const std::string& s) -> bool {
        auto dash = s.find('-');
        return dash != std::string::npos &&
               dash > 0 &&
               std::all_of(s.begin(), s.begin() + dash,
                           [](unsigned char c){ return std::isdigit(c); }) &&
               std::all_of(s.begin() + dash + 1, s.end(),
                           [](unsigned char c){ return std::isdigit(c); });
    };

    // helper: split token into  {string x, size_t y}
    auto parseAgent = [](const std::string& s) -> std::pair<std::string, size_t> {
        std::size_t dash = s.find('-');
        std::string x    = s.substr(0, dash);          // keep as string
        size_t      y    = std::stoul(s.substr(dash + 1));
        return {x, y};
    };


    std::string line;
    while (std::getline(file, line))
    {
        if (line.empty()) continue;

        std::istringstream iss(line);
        char tag;  iss >> tag;

        if (tag == '*') {
            std::string nodeLabel;  iss >> nodeLabel;
            graph[nodeLabel];

            std::string token;
            while (iss >> token) {
                if (isAgent(token)) {
                    auto [x, y] = parseAgent(token);
                    initialAgentsData.push_back({x, y, nodeLabel});
                }
            }
        }
        else if (tag == '-') {
            std::string src, dst; size_t w;
            iss >> src >> dst >> w;
            graph[src].emplace_back(Edge{dst, w});
        }
    }
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

std::vector<Edge> Graph::getNeighbors(const std::string& node,
                                      size_t weight) const
{
    std::vector<Edge> result;

    auto it = graph.find(node);
    if (it == graph.end()) return result;

    const std::vector<Edge>& adj = it->second;
    for (const Edge& e : adj) {
        if (e.weight == weight)
            result.push_back(e);
    }
    return result;
}

#pragma endregion





