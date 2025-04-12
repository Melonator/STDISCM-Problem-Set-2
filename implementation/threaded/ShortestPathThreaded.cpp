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

struct State {
    std::string node;
    size_t weight;
    std::vector<std::string> path;
};

std::vector<std::string> ShortestPathThreaded::displayPath(const std::string &start, const std::string &end, Graph* graph) {
    std::atomic<size_t> bestWeight(SIZE_MAX);
    std::mutex bestMutex;
    std::vector<std::string> bestPath;

    // Recursive lambda that explores from a given state.
    // The "depth" parameter helps limit immediate recursion (to control thread spawning).
    std::function<void(State, size_t)> processState;
    processState = [&](State current, int depth) {
        // Prune if the current state's weight is not better than the best found.
        if (current.weight >= bestWeight.load())
            return;
        if (current.node == end) {
            size_t prevBest = bestWeight.load();
            while (current.weight < prevBest && !bestWeight.compare_exchange_weak(prevBest, current.weight)) {
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
            if (next.weight >= bestWeight.load())
                continue;
            // Limit recursion
            if (depth >= 3) {
                futures.push_back(std::async(std::launch::async, processState, next, depth + 1));
            } else {
                processState(next, depth + 1);
            }
        }
        // Finish all spawned tasks (those that exceeded the depth)
        for (auto &f : futures) {
            f.get();
        }
    };

    State initial { start, 0, {start} };
    auto fut = std::async(std::launch::async, processState, initial, 0);
    fut.get();

    // Output the best solution if found.
    if (bestWeight.load() < SIZE_MAX) {
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

    return bestPath;
}