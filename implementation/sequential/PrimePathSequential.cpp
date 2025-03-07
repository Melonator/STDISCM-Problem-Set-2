#include "../../headers/sequential/PrimePathSequential.h"
#include "../../headers/Graph.h"
#include <iostream>
#include <queue>
#include <vector>
#include <string>

// Helper function to check if a number is prime.
bool isPrime(int n) {
    if(n <= 1) return false;
    if(n <= 3) return true;
    if(n % 2 == 0 || n % 3 == 0) return false;
    for (int i = 5; i * i <= n; i += 6) {
         if(n % i == 0 || n % (i+2) == 0)
             return false;
    }
    return true;
}

// Structure representing a search state.
struct State {
    std::string node;
    int weight;
    std::vector<std::string> path;
};

void PrimePathSequential::displayPath(const std::string &start, const std::string &end, Graph* graph) {
    std::queue<State> q;
    q.push({start, 0, {start}});
    bool found = false;

    while (!q.empty()) {
         State current = q.front();
         q.pop();

         // If we reach the destination with a non-zero prime total weight.
         if (current.node == end && current.weight > 0 && isPrime(current.weight)) {
              std::cout << "prime path: ";
              for (size_t i = 0; i < current.path.size(); ++i) {
                  std::cout << current.path[i];
                  if(i < current.path.size() - 1)
                      std::cout << " -> ";
              }
              std::cout << " with weight/length= " << current.weight << "\n";
              found = true;
              break;
         }

         // Expand each neighbor from the current node.
         const std::vector<Edge>& neighbors = graph->getNeighbors(current.node);
         for (const Edge &edge : neighbors) {
              // Check if the neighbor is already in the path (avoid cycles).
              bool alreadyVisited = false;
              for (const auto &n : current.path) {
                  if(n == edge.node) {
                      alreadyVisited = true;
                      break;
                  }
              }
              if (alreadyVisited)
                  continue;

              State next;
              next.node = edge.node;
              next.weight = current.weight + edge.weight;
              next.path = current.path;
              next.path.push_back(edge.node);
              q.push(next);
         }
    }

    if (!found) {
         std::cout << "No prime path from " << start << " to " << end << "\n";
    }
}