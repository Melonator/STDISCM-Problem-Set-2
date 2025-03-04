#include <iostream>

#include "headers/Graph.h"
#include "headers/Pathfinder.h"
#include "headers/ThreadPool.h"
#include "headers/sequential/AStarSequential.h"
#include "headers/sequential/BFSSequential.h"
#include "headers/sequential/PrimePathSequential.h"
#include "headers/sequential/ShortestPrimePathSequential.h"

using namespace std;

int main() {
    Pathfinder pathFinder;
    string file_name;
    string fullCommand;
    std::unique_ptr<IPathBuilder> aStar;
    std::unique_ptr<IPathBuilder> bfs;
    std::unique_ptr<IPathBuilder> primePath;
    std::unique_ptr<IPathBuilder> shortestPrimePath;

    int mode = 0;
    cout << "=== G R A P H  M A N I A ===" << "\n\n";
    cout << "Enter the filename:";
    cin >> file_name;
    Graph* graph;
    cout << "[0] Serial Mode, [1] Parallel Mode:";
    cin >> mode;

    if (mode == 0) {
        graph = new Graph(file_name);
        aStar = std::make_unique<AStarSequential>();
        bfs = std::make_unique<BFSSequential>();
        primePath = std::make_unique<PrimePathSequential>();
        shortestPrimePath = std::make_unique<ShortestPrimePathSequential>();
    } else {
        int numThreads = 0;
        cout << "How many threads?";
        cin >> numThreads;
    }

    cin.ignore();
    while (fullCommand != "exit") {
        cout << "Enter a command:";
        getline(cin, fullCommand);
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
            cout << output << node1 << " "<< node2 << "\n";
        } else if (command == "path") {
            const string node1 = fullCommand.substr(5, fullCommand.find(' ', 5) - 5);
            const string node2 = fullCommand.substr(node1.length() + 6);
            pathFinder.setStrategy(bfs.get());
            pathFinder.displayPath(node1, node2);
        } else if (command == "prime-path") {
            const string node1 = fullCommand.substr(5, fullCommand.find(' ', 5) - 5);
            const string node2 = fullCommand.substr(node1.length() + 6);
            pathFinder.setStrategy(primePath.get());
            pathFinder.displayPath(node1, node2);
        } else if (command == "shortest-prime-path") {
            const string node1 = fullCommand.substr(5, fullCommand.find(' ', 5) - 5);
            const string node2 = fullCommand.substr(node1.length() + 6);
            pathFinder.setStrategy(shortestPrimePath.get());
            pathFinder.displayPath(node1, node2);
        } else if (command == "shortest-path") {
            const string node1 = fullCommand.substr(5, fullCommand.find(' ', 5) - 5);
            const string node2 = fullCommand.substr(node1.length() + 6);
            pathFinder.setStrategy(aStar.get());
            pathFinder.displayPath(node1, node2);
        }
    }
    return 0;
}
