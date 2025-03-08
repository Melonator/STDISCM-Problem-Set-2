#include "../../headers/threaded/PrimePathThreaded.h"
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
bool PrimePathThreaded::isPrime(int n) {
    if(n <= 1) return false;
    if(n <= 3) return true;
    if(n % 2 == 0 || n % 3 == 0) return false;
    for (int i = 5; i * i <= n; i += 6)
         if(n % i == 0 || n % (i+2) == 0)
             return false;
    return true;
}


void PrimePathThreaded::displayPath(const std::string &start, const std::string &end, Graph* graph) {
    // Create a thread pool. Use hardware concurrency if available; otherwise default to 4.
    size_t numThreads = std::thread::hardware_concurrency();
    if(numThreads == 0) numThreads = 4;
    ThreadPool pool(numThreads);

    std::atomic<bool> found(false);
    std::mutex outputMutex; // To serialize output (only one result is printed).

    // Recursive lambda to process a state.
    std::function<void(State)> processState;
    processState = [&](State current) {
        if (found.load()) return; // If already found, skip processing further.
        // Check if we've reached the destination with a prime (nonzero) weight.
        if (current.node == end && current.weight > 0 && isPrime(current.weight)) {
            // Use compare_exchange to ensure only one thread prints the result.
            bool expected = false;
            if (found.compare_exchange_strong(expected, true)) {
                std::lock_guard<std::mutex> lock(outputMutex);
                std::cout << "prime path: ";
                for (size_t i = 0; i < current.path.size(); ++i) {
                    std::cout << current.path[i];
                    if(i < current.path.size() - 1)
                        std::cout << " -> ";
                }
                std::cout << " with weight/length= " << current.weight << "\n";
            }
            return;
        }
        // Expand neighbors.
        const std::vector<Edge>& neighbors = graph->getNeighbors(current.node);
        for (const Edge &edge : neighbors) {
            // Avoid cycles by checking if this neighbor is already in the path.
            bool alreadyVisited = false;
            for (const auto &n : current.path) {
                if(n == edge.node) {
                    alreadyVisited = true;
                    break;
                }
            }
            if (alreadyVisited)
                continue;
            // Create next state.
            State next;
            next.node = edge.node;
            next.weight = current.weight + edge.weight;
            next.path = current.path;
            next.path.push_back(edge.node);
            // Submit the next state to the thread pool.
            pool.queueTask([=, &found, &processState]() {
                if (!found.load()) {
                    processState(next);
                }
            });
        }
    };

    // Start by queuing the initial state.
    State initial { start, 0, {start} };
    pool.queueTask([&]() { processState(initial); });
    pool.waitForAllTasks();

    if (!found.load()) {
         std::cout << "No prime path from " << start << " to " << end << "\n";
    }
}