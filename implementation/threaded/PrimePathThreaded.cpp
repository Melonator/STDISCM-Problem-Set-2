#include "../../headers/threaded/PrimePathThreaded.h"
#include "../../headers/Graph.h"
#include <iostream>
#include <vector>
#include <string>
#include <atomic>
#include <mutex>
#include <future>
#include <climits>
#include <cmath>

bool PrimePathThreaded::isPrime(size_t n) {
    if(n <= 1) return false;
    if(n <= 3) return true;
    if(n % 2 == 0 || n % 3 == 0) return false;
    for (size_t i = 5; i * i <= n; i += 6)
         if(n % i == 0 || n % (i+2) == 0)
             return false;
    return true;
}

std::vector<std::string> PrimePathThreaded::displayPath(const std::string &start, const std::string &end, Graph* graph) {
    std::atomic<bool> found(false);
    std::mutex outputMutex;
    std::vector<std::string> finalPath;

    // Recursive lambda using std::async for parallelism.
    std::function<void(State, size_t)> processState;
    processState = [&](State current, int depth) {
        if (found.load()) return;
        if (current.node == end && current.weight > 0 && isPrime(current.weight)) {
            bool expected = false;
            if (found.compare_exchange_strong(expected, true)) {
                std::lock_guard<std::mutex> lock(outputMutex);
                finalPath = current.path;
                std::cout << "prime path: ";
                for (size_t i = 0; i < current.path.size(); ++i) {
                    std::cout << current.path[i];
                    if (i < current.path.size() - 1)
                        std::cout << " -> ";
                }
                std::cout << " with weight/length= " << current.weight << "\n";
            }
        }

        const std::vector<Edge>& neighbors = graph->getNeighbors(current.node);
        std::vector<std::future<void>> localFutures;
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
            // Limit recursion
            if (depth >= 3) {
                localFutures.push_back(std::async(std::launch::async, processState, next, depth + 1));
            } else {
                processState(next, depth + 1);
            }
        }
        // Finish all spawned tasks (those that exceeded the depth)
        for (auto &fut : localFutures) {
            fut.get();
        }
    };

    // Begin with the initial state.
    State initial { start, 0, {start} };
    auto fut = std::async(std::launch::async, processState, initial, 0);
    fut.get();

    if (!found.load()) {
         std::cout << "No prime path from " << start << " to " << end << "\n";
    }

    return finalPath;
}