#include "../../headers/threaded/ShortestPrimePathThreaded.h"
#include "../../headers/Graph.h"
#include <iostream>
#include <vector>
#include <string>
#include <atomic>
#include <mutex>
#include <future>
#include <climits>
#include <cmath>
#include <functional>

// For convenience, alias the State type.
using State = ShortestPrimePathThreaded::State;

bool ShortestPrimePathThreaded::isPrime(int n) {
    if(n <= 1) return false;
    if(n <= 3) return true;
    if(n % 2 == 0 || n % 3 == 0) return false;
    for (int i = 5; i * i <= n; i += 6)
         if(n % i == 0 || n % (i + 2) == 0)
             return false;
    return true;
}

void ShortestPrimePathThreaded::displayPath(const std::string &start, const std::string &end, Graph* graph) {
    // Shared atomic for the best (smallest) prime weight found.
    std::atomic<int> bestWeight(INT_MAX);
    // Mutex for protecting the best path update.
    std::mutex bestMutex;
    std::vector<std::string> bestPath;

    // Recursive lambda that searches from a given state.
    // The "depth" parameter is used to limit immediate recursion.
    std::function<void(State, int)> processState;
    processState = [&](State current, int depth) {
        // Prune if the current weight is no better than the best found.
        if (current.weight >= bestWeight.load())
            return;
        // If destination reached with a valid prime weight, try to update best solution.
        if (current.node == end && current.weight > 0 && isPrime(current.weight)) {
            int prevBest = bestWeight.load();
            // Atomically update bestWeight.
            while (current.weight < prevBest && !bestWeight.compare_exchange_weak(prevBest, current.weight)) {
                // prevBest is updated with the current best value.
            }
            {
                std::lock_guard<std::mutex> lock(bestMutex);
                bestPath = current.path;
            }
            return;
        }
        // Expand neighbors from the current node.
        const std::vector<Edge>& neighbors = graph->getNeighbors(current.node);
        std::vector<std::future<void>> futures;
        for (const Edge &edge : neighbors) {
            // Skip if the neighbor is already in the path (avoid cycles).
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
            if (next.weight >= bestWeight.load())
                continue;
            // If the recursion depth is too high, launch asynchronously.
            if (depth >= 3) {
                futures.push_back(std::async(std::launch::async, processState, next, depth + 1));
            } else {
                processState(next, depth + 1);
            }
        }
        // Wait for all asynchronous tasks at this level.
        for (auto &f : futures) {
            f.get();
        }
    };

    // Start processing from the initial state.
    State initial { start, 0, {start} };
    auto fut = std::async(std::launch::async, processState, initial, 0);
    fut.get();

    // Output the best solution if found.
    if (bestWeight.load() < INT_MAX) {
         std::cout << "shortest prime path: ";
         for (size_t i = 0; i < bestPath.size(); ++i) {
             std::cout << bestPath[i];
             if(i < bestPath.size() - 1)
                  std::cout << " -> ";
         }
         std::cout << " with weight/length= " << bestWeight.load() << "\n";
    } else {
         std::cout << "No prime path from " << start << " to " << end << "\n";
    }
}