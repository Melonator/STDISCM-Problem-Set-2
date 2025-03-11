// File: implementation/sequential/ShortestPathSequential.cpp

#include "../../headers/sequential/ShortestPathSequential.h"
#include "../../headers/Graph.h"
#include <iostream>
#include <queue>
#include <vector>
#include <string>

void ShortestPathSequential::displayPath(const std::string &start, const std::string &end, Graph* graph) {
    // Priority queue to select the state with the smallest cumulative weight.
    std::priority_queue<State, std::vector<State>, StateComparator> pq;
    pq.push({start, 0, {start}});

    bool found = false;
    
    // Dijkstra-like loop.
    while (!pq.empty()) {
        State current = pq.top();
        pq.pop();
        
        // If destination is reached, output the path and weight.
        if (current.node == end) {
            std::cout << "shortest path: ";
            for (size_t i = 0; i < current.path.size(); ++i) {
                std::cout << current.path[i];
                if (i < current.path.size() - 1)
                    std::cout << " -> ";
            }
            std::cout << " with weight/length= " << current.weight << "\n";
            found = true;
            break;
        }
        
        // Expand neighbors.
        const std::vector<Edge>& neighbors = graph->getNeighbors(current.node);
        for (const Edge &edge : neighbors) {
            // Avoid cycles by ensuring we don't revisit nodes already in the path.
            bool alreadyVisited = false;
            for (const auto &visitedNode : current.path) {
                if (visitedNode == edge.node) {
                    alreadyVisited = true;
                    break;
                }
            }
            if (alreadyVisited)
                continue;
            
            // Create the next state.
            State next;
            next.node = edge.node;
            next.weight = current.weight + edge.weight;
            next.path = current.path;
            next.path.push_back(edge.node);
            
            pq.push(next);
        }
    }
    
    if (!found) {
        std::cout << "No path from " << start << " to " << end << "\n";
    }
}