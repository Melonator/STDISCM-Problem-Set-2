#include "../../headers/sequential/BFSSequential.h"
#include "../../headers/Graph.h"
#include <iostream>
#include <queue>
#include <vector>
#include <string>

void BFSSequential::displayPath(const std::string &start, const std::string &end, Graph* graph) {
    // Define a local structure to hold the state for the BFS.
    struct State {
        std::string node;              // current node
        size_t weight;                    // cumulative weight from the start node
        std::vector<std::string> path; // path taken so far
    };

    std::queue<State> q;
    // Start with the initial state: start node, zero weight, and path containing only start.
    q.push({start, 0, {start}});
    bool found = false;

    while (!q.empty()) {
        State current = q.front();
        q.pop();

        // If destination is reached, output the path and its cumulative weight.
        if (current.node == end) {
            std::cout << "path: ";
            for (size_t i = 0; i < current.path.size(); i++) {
                std::cout << current.path[i];
                if (i < current.path.size() - 1)
                    std::cout << " -> ";
            }
            std::cout << " with weight/length= " << current.weight << "\n";
            found = true;
            break;
        }

        // Expand neighbors of the current node.
        const std::vector<Edge>& neighbors = graph->getNeighbors(current.node);
        for (const Edge &edge : neighbors) {
            // Avoid cycles by checking if the neighbor is already in the path.
            bool alreadyVisited = false;
            for (const auto &visitedNode : current.path) {
                if (visitedNode == edge.node) {
                    alreadyVisited = true;
                    break;
                }
            }
            if (alreadyVisited)
                continue;

            // Create a new state for the neighbor.
            State next;
            next.node = edge.node;
            next.weight = current.weight + edge.weight;
            next.path = current.path;
            next.path.push_back(edge.node);
            q.push(next);
        }
    }

    if (!found) {
        std::cout << "No path from " << start << " to " << end << "\n";
    }
}