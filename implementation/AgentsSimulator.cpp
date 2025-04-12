#include "../headers/AgentsSimulator.h"

#include <algorithm>
#include <iomanip>

#include "../headers/InitialAgentData.h"

#include <iostream>
#include <queue>
#include <thread>

std::mutex mtx;
std::mutex displayMtx;
struct State {
    std::string node;              // Current node label.
    size_t weight;                    // Cumulative weight from the start node.
    std::vector<std::string> path; // The path taken so far.
};

struct StateComparator {
    bool operator()(const State &a, const State &b) const {
        return a.weight > b.weight;
    }
};

#include <chrono>
#include <format>
#include <iostream>

void printLogEntry(const LogEntry &entry) {
    // Convert to nanoseconds since epoch
    auto ns_since_epoch = std::chrono::duration_cast<std::chrono::nanoseconds>(
        entry.timestamp.time_since_epoch()
    ).count();

    // Format as human-readable (C++20)
    std::string human_time = std::format("{:%Y-%m-%d %H:%M:%S}", entry.timestamp);

    std::cout << ns_since_epoch << "ns [" << human_time << "] "
              << entry.agent << " "
              << entry.state << " node "
              << entry.nodeName << std::endl;
}

AgentsSimulator::AgentsSimulator(Graph *graph): graph(graph) {}

void AgentsSimulator::runSimulation() {
    //Initialize Paths
    auto initialAgentsData = graph->getInitialAgentsData();
    std::vector<std::vector<LogEntry>> logsPerAgent(initialAgentsData.size());

    int i = 0;
    for (auto initialAgentData : initialAgentsData) {
        agentPaths.emplace_back(findPath(initialAgentData.startNode, initialAgentData.x, stoi(initialAgentData.x), i));
        i++;
    }

    //Initialize Node Monitors
    i = 0;
    for (auto path : agentPaths) {
        for (auto node: path) {
            if (nodeMonitors.contains(node)) {
                auto monitor = nodeMonitors.at(node);
                monitor->addToQueue(i);
            } else {
                NodeMonitor *nodeMonitor = new NodeMonitor();
                nodeMonitor->addToQueue(i);
                nodeMonitors[node] = std::move(nodeMonitor);
            }
        }
        i++;
    }

    {
        std::vector<std::jthread> agentThreads(initialAgentsData.size());
        for (i = 0; i < initialAgentsData.size(); i++) {
            agentThreads.emplace_back(&AgentsSimulator::explore, this, i, initialAgentsData[i], std::ref(logsPerAgent[i]));
        }
    }

    //display sorted logs!
    std::vector<LogEntry> allLogs;
    for (const auto &agentLogs : logsPerAgent) {
        allLogs.insert(allLogs.end(), agentLogs.begin(), agentLogs.end());
    }

    // Optional: sort the logs by timestamp if needed.
    std::sort(allLogs.begin(), allLogs.end(), [](const LogEntry &a, const LogEntry &b) {
        return a.timestamp < b.timestamp;
    });

    // Display the logs.
    std::cout << "\n------ Merged Log Report ------\n";
    for (const auto &entry : allLogs) {
        printLogEntry(entry);
    }

    // Clean up dynamically allocated NodeMonitors.
    for (auto &pair : nodeMonitors) {
        delete pair.second;
    }
}

void AgentsSimulator::explore(int agent_id, InitialAgentData agent_data, std::vector<LogEntry>& log_entries) const {

    std::vector<std::string> path;
    {
        std::lock_guard lock(mtx);
        path = agentPaths[agent_id];
    }

    std::stringstream agentStream;
    agentStream << "Agent " << agent_data.x << "-" << agent_data.y;
    std::string agentName = agentStream.str();

    NodeMonitor* prevMonitor = nullptr;
    std::string prevNodeName = "";
    for (const auto &nodeName : path) {
        // Get the current node's monitor.
        NodeMonitor* currMonitor = nullptr;
        {
            std::lock_guard lock(mtx);
            currMonitor = nodeMonitors.at(nodeName);
        }

        // Log that the agent is waiting for the current node.
        log_entries.push_back({ std::chrono::system_clock::now(), agentName, "is waiting for", nodeName });
        // Wait until it's the agent's turn on the current node.
        currMonitor->waitForTurn(agent_id);

        // Once we've waited for the current node's turn, release the previous node if one exists.
        if (prevMonitor != nullptr) {
            log_entries.push_back({ std::chrono::system_clock::now(), agentName, "released", prevNodeName });
            prevMonitor->releaseTurn();
        }

        // Now that we've released the previous node, log that the agent is at the current node.
        log_entries.push_back({ std::chrono::system_clock::now(), agentName, "is at", nodeName });
        // Simulate exploring the node.
        std::this_thread::sleep_for(std::chrono::milliseconds(0));

        // Update the previous node to be the current node.
        prevMonitor   = currMonitor;
        prevNodeName  = nodeName;
    }

    // After processing all nodes, release the last node.
    if (prevMonitor != nullptr) {
        log_entries.push_back({ std::chrono::system_clock::now(), agentName, "released", prevNodeName });
        prevMonitor->releaseTurn();
    }
}

std::vector<std::string> AgentsSimulator::findPath(std::string start, std::string end, size_t weight, int agent_id) {
    std::priority_queue<State, std::vector<State>, StateComparator> pq;
    std::vector<std::string> finalPath;

    pq.push({start, 0, {start}});

    bool found = false;

    while (!pq.empty()) {
        State current = pq.top();
        pq.pop();

        // If destination is reached, output the path and weight.
        if (current.node == end) {
            finalPath = current.path;
            std::cout << "shortest path: ";
            for (size_t i = 0; i < current.path.size(); ++i) {
                std::cout << current.path[i];
                if (i < current.path.size() - 1)
                    std::cout << " -> ";
            }
            std::cout << " with weight/length= " << current.weight << "\n";
            found = true;
            break;
        }

        // Expand neighbors.
        const std::vector<Edge>& neighbors = graph->getNeighbors(current.node, weight);
        for (const Edge &edge : neighbors) {
            bool alreadyVisited = false;
            for (const auto &visitedNode : current.path) {
                if (visitedNode == edge.node) {
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

            pq.push(next);
        }
    }

    if (!found) {
        std::cout << "No path from " << start << " to " << end << "\n";
    }

    return finalPath;
}



