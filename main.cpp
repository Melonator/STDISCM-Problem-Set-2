#include <iostream>
#include <memory>
#include <string>
#include <chrono>

// Include both sequential and threaded headers.
#include "headers/Graph.h"
#include "headers/Pathfinder.h"
#include "headers/ThreadPool.h"

// Sequential implementations.
#include "headers/sequential/ShortestPathSequential.h"
#include "headers/sequential/BFSSequential.h"
#include "headers/sequential/PrimePathSequential.h"
#include "headers/sequential/ShortestPrimePathSequential.h"

// Threaded implementations.
#include "headers/AgentsSimulator.h"
#include "headers/threaded/ShortestPathThreaded.h"
#include "headers/threaded/BFSThreaded.h"
#include "headers/threaded/PrimePathThreaded.h"
#include "headers/threaded/ShortestPrimePathThreaded.h"

using namespace std;

int main() {
    string file_name;
    string fullCommand;

    // Algorithm strategies that could be threaded or sequential
    std::unique_ptr<IPathBuilder> shortestPath;
    std::unique_ptr<IPathBuilder> bfs;
    std::unique_ptr<IPathBuilder> primePath;
    std::unique_ptr<IPathBuilder> shortestPrimePath;

    int mode = 0;
    cout << "=== G R A P H  M A N I A ===" << "\n\n";
    cout << "Enter the filename: ";
    cin >> file_name;

    Graph* graph = nullptr;
    // Create the Pathfinder using the graph pointer.
    Pathfinder pathFinder(nullptr);

    cout << "[0] Serial Mode, [1] Parallel Mode: ";
    cin >> mode;

    if (mode == 0) {
        // Sequential mode.
        graph = new Graph(file_name);
        shortestPath = std::make_unique<ShortestPathSequential>();
        bfs = std::make_unique<BFSSequential>();
        primePath = std::make_unique<PrimePathSequential>();
        shortestPrimePath = std::make_unique<ShortestPrimePathSequential>();
    } else {
        // Parallel mode.
        graph = new Graph(file_name);
        shortestPath = std::make_unique<ShortestPathThreaded>();
        bfs = std::make_unique<BFSThreaded>();
        primePath = std::make_unique<PrimePathThreaded>();
        shortestPrimePath = std::make_unique<ShortestPrimePathThreaded>();
    }

    pathFinder = Pathfinder(graph);
    AgentsSimulator agentsSimulator(graph);

    cin.ignore();
    while (true) {
        cout << "Enter a command: ";
        getline(cin, fullCommand);
        if (fullCommand == "exit")
            break;

        // Get the first token of the command.
        const string command = fullCommand.substr(0, fullCommand.find(' '));

        if (command == "node") {
            const string node1 = fullCommand.substr(5, fullCommand.find(' ', 5) - 5);
            const bool result = graph->hasNode(node1);
            const string output = result ? "The graph has node " : "The graph DOES NOT have node ";
            cout << output << node1 << "\n";
        } else if (command == "edge") {
            const string node1 = fullCommand.substr(5, fullCommand.find(' ', 5) - 5);
            const string node2 = fullCommand.substr(node1.length() + 6);
            const bool result = graph->hasEdge(node1, node2);
            const string output = result ? "The graph has edge " : "The graph DOES NOT have edge ";
            cout << output << node1 << " " << node2 << "\n";
        } else if (command == "path") {
            // Format: path node1 node2
            const string node1 = fullCommand.substr(5, fullCommand.find(' ', 5) - 5);
            const string node2 = fullCommand.substr(node1.length() + 6);
            pathFinder.setStrategy(bfs.get());
            pathFinder.displayPath(node1, node2, graph);
        } else if (command == "prime-path") {
            // Format: prime-path node1 node2
            size_t firstSpace = fullCommand.find(' ');
            size_t secondSpace = fullCommand.find(' ', firstSpace + 1);
            string node1 = fullCommand.substr(firstSpace + 1, secondSpace - firstSpace - 1);
            string node2 = fullCommand.substr(secondSpace + 1);
            pathFinder.setStrategy(primePath.get());
            pathFinder.displayPath(node1, node2, graph);
        } else if (command == "shortest-prime-path") {
            // Format: shortest-prime-path node1 node2
            size_t firstSpace = fullCommand.find(' ');
            size_t secondSpace = fullCommand.find(' ', firstSpace + 1);
            string node1 = fullCommand.substr(firstSpace + 1, secondSpace - firstSpace - 1);
            string node2 = fullCommand.substr(secondSpace + 1);
            pathFinder.setStrategy(shortestPrimePath.get());
            pathFinder.displayPath(node1, node2, graph);
        } else if (command == "shortest-path") {
            // Format: shortest-path node1 node2
            size_t firstSpace = fullCommand.find(' ');
            size_t secondSpace = fullCommand.find(' ', firstSpace + 1);
            string node1 = fullCommand.substr(firstSpace + 1, secondSpace - firstSpace - 1);
            string node2 = fullCommand.substr(secondSpace + 1);
            pathFinder.setStrategy(shortestPath.get());
            pathFinder.displayPath(node1, node2, graph);
        } else if (command == "simulate-agents") {
            agentsSimulator.RunSimulation();
        } else {
            cout << "Invalid command.\n";
        }
    }

    // Cleanup
    delete graph;
    return 0;
}