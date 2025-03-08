// File: implementation/threaded/ShortestPrimePathThreaded.cpp

#include "../../headers/threaded/ShortestPrimePathThreaded.h"
#include "../../headers/Graph.h"
#include "../../headers/ThreadPool.h"
#include <iostream>
#include <vector>
#include <string>
#include <atomic>
#include <mutex>
#include <thread>
#include <functional>
#include <climits>
#include <cmath>

// Helper function to check if a number is prime.
bool ShortestPrimePathThreaded::isPrime(int n) {
    if(n <= 1) return false;
    if(n <= 3) return true;
    if(n % 2 == 0 || n % 3 == 0) return false;
    for (int i = 5; i * i <= n; i += 6)
         if(n % i == 0 || n % (i+2) == 0)
             return false;
    return true;
}

void ShortestPrimePathThreaded::displayPath(const std::string &start, const std::string &end, Graph* graph) {
    // Create a thread pool using available hardware concurrency (or default to 4 if not available).
    size_t numThreads = std::thread::hardware_concurrency();
    if(numThreads == 0) numThreads = 4;
    ThreadPool pool(numThreads);

    // Shared variables for tracking the best (smallest) prime weight found.
    std::atomic<int> bestWeight(INT_MAX);
    std::mutex bestMutex; // Protects updates to bestPath.
    std::vector<std::string> bestPath;

    // Recursive lambda with a depth parameter to avoid unbounded recursion.
    std::function<void(State, int)> processState;
    processState = [&](State current, int depth) {
        // Prune if current state's weight already exceeds the best found.
        if (current.weight >= bestWeight.load())
            return;
        // Check if we have reached the destination with a prime weight.
        if (current.node == end && current.weight > 0 && isPrime(current.weight)) {
            int prevBest = bestWeight.load();
            // Update bestWeight if the current weight is smaller.
            while (current.weight < prevBest && !bestWeight.compare_exchange_weak(prevBest, current.weight)) {
                // prevBest is updated with the current best value.
            }
            {
                std::lock_guard<std::mutex> lock(bestMutex);
                bestPath = current.path;
            }
            return;
        }
        // Expand neighbors.
        const std::vector<Edge>& neighbors = graph->getNeighbors(current.node);
        for (const Edge &edge : neighbors) {
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
            // Prune if next state's weight is already no better than the best found.
            if (next.weight >= bestWeight.load())
                continue;
            // If depth exceeds threshold, queue a new task with depth reset to 0.
            if (depth >= 50) {
                pool.queueTask([=, &bestWeight, &processState]() {
                    processState(next, 0);
                });
            } else {
                processState(next, depth + 1);
            }
        }
    };

    // Start processing from the initial state.
    State initial { start, 0, {start} };
    pool.queueTask([&]() { processState(initial, 0); });
    pool.waitForAllTasks();

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