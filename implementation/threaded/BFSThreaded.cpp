#include "../../headers/threaded/BFSThreaded.h"
#include "../../headers/Graph.h"
#include <iostream>
#include <vector>
#include <string>
#include <future>
#include <mutex>
#include <atomic>

std::vector<std::string> BFSThreaded::displayPath(const std::string &start, const std::string &end, Graph* graph) {
    struct State {
        std::string node;
        size_t weight;
        std::vector<std::string> path;
    };

    std::vector<std::string> finalPath;
    std::atomic<bool> found(false);
    State foundState;
    std::mutex foundMutex;

    std::vector<State> currentFrontier;
    currentFrontier.push_back({start, 0, {start}});

    // Process levels until either the frontier is empty or the destination is found.
    while (!currentFrontier.empty() && !found.load()) {
        std::vector<State> nextFrontier;
        std::vector<std::future<std::vector<State>>> futures;

        for (auto &state : currentFrontier) {
            futures.push_back(std::async(std::launch::async, [&graph, &state, &end, &found]() -> std::vector<State> {
                std::vector<State> newStates;
                const std::vector<Edge>& neighbors = graph->getNeighbors(state.node);
                for (const Edge &edge : neighbors) {
                    bool alreadyVisited = false;
                    for (const auto &n : state.path) {
                        if (n == edge.node) {
                            alreadyVisited = true;
                            break;
                        }
                    }
                    if (alreadyVisited)
                        continue;
                    State next;
                    next.node = edge.node;
                    next.weight = state.weight + edge.weight;
                    next.path = state.path;
                    next.path.push_back(edge.node);
                    newStates.push_back(next);
                }
                return newStates;
            }));
        }

        // Gather results from all tasks.
        for (auto &fut : futures) {
            std::vector<State> newStates = fut.get();
            for (auto &s : newStates) {
                if (s.node == end) {
                    if (!found.load()) {
                        std::lock_guard<std::mutex> lock(foundMutex);
                        if (!found.load()) { // Double-check inside the lock.
                            found.store(true);
                            foundState = s;
                        }
                    }
                }
                nextFrontier.push_back(s);
            }
        }

        if (found.load())
            break;
        currentFrontier = std::move(nextFrontier);
    }

    // Output the result.
    if (found.load()) {
        std::cout << "path: ";
        for (size_t i = 0; i < foundState.path.size(); ++i) {
            std::cout << foundState.path[i];
            if (i < foundState.path.size() - 1)
                std::cout << " -> ";
        }
        std::cout << " with weight/length= " << foundState.weight << "\n";
    } else {
        std::cout << "No path from " << start << " to " << end << "\n";
    }

    return foundState.path;
}