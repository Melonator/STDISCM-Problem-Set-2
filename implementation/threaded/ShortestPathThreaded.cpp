#include "../../headers/threaded/ShortestPathThreaded.h"
#include "../../headers/Graph.h"
#include <iostream>
#include <vector>
#include <string>
#include <atomic>
#include <mutex>
#include <future>
#include <climits>
#include <functional>

// Define a local structure to represent a search state.
struct State {
    std::string node;              // Current node label.
    size_t weight;                    // Cumulative weight from the start.
    std::vector<std::string> path; // The path taken so far.
};

void ShortestPathThreaded::displayPath(const std::string &start, const std::string &end, Graph* graph) {
    // Shared atomic to hold the best (lowest) cumulative weight found.
    std::atomic<int> bestWeight(INT_MAX);
    // Mutex to protect bestPath updates.
    std::mutex bestMutex;
    std::vector<std::string> bestPath;

    // Recursive lambda that explores from a given state.
    // The "depth" parameter helps limit immediate recursion (to control thread spawning).
    std::function<void(State, int)> processState;
    processState = [&](State current, int depth) {
        // Prune if the current state's weight is not better than the best found.
        if (current.weight >= bestWeight.load())
            return;
        // If we've reached the destination, update the best solution.
        if (current.node == end) {
            int prevBest = bestWeight.load();
            while (current.weight < prevBest && !bestWeight.compare_exchange_weak(prevBest, current.weight)) {
                // prevBest is updated with the latest best value.
            }
            {
                std::lock_guard<std::mutex> lock(bestMutex);
                bestPath = current.path;
            }
            return;
        }
        // Expand neighbors.
        const std::vector<Edge>& neighbors = graph->getNeighbors(current.node);
        std::vector<std::future<void>> futures;
        for (const Edge &edge : neighbors) {
            // Avoid cycles: skip neighbor if already in current path.
            bool alreadyVisited = false;
            for (const auto &n : current.path) {
                if (n == edge.node) {
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
            // Prune if next state's weight is no better than the current best.
            if (next.weight >= bestWeight.load())
                continue;
            // To limit recursion depth, if depth exceeds threshold, spawn asynchronously.
            if (depth >= 3) {
                futures.push_back(std::async(std::launch::async, processState, next, depth + 1));
            } else {
                processState(next, depth + 1);
            }
        }
        // Wait for all spawned asynchronous tasks.
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
         std::cout << "shortest path: ";
         for (size_t i = 0; i < bestPath.size(); ++i) {
             std::cout << bestPath[i];
             if(i < bestPath.size() - 1)
                  std::cout << " -> ";
         }
         std::cout << " with weight/length= " << bestWeight.load() << "\n";
    } else {
         std::cout << "No path from " << start << " to " << end << "\n";
    }
}