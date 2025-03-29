#include "../headers/AgentsSimulator.h"
#include "../headers/InitialAgentData.h"

#include <iostream>
#include <thread>


std::mutex m;
AgentsSimulator::AgentsSimulator(Graph *graph): graph(graph) {
    auto nodes = this->graph->getNodes();
    for (const auto& node : nodes) {
        graphMutexes[node] = new std::mutex();
    }
}

//TODO: Delegate 1 agent to 1 explore() function
void AgentsSimulator::RunSimulation() {
    //Sample code
    InitialAgentData initial_agent_data1 {
        1,
        3,
        "a"
    };


    InitialAgentData initial_agent_data2 {
        1,
        2,
        "a"
    };

    InitialAgentData initial_agent_data3 {
        1,
        2,
        "b"
    };

    std::thread t1(Explore, this ,initial_agent_data1);
    std::thread t2(Explore, this, initial_agent_data2);
    std::thread t3(Explore, this, initial_agent_data3);

    t1.join();
    t2.join();
    t3.join();
}


//TODO: bfs/dfs style exploration
// Use the graphMutexes map to halt exploration (pop from the queue, then try to acquire the node's mutex
// Use std::timed_mutex to avoid deadlock situation (try to acquire for n seconds, else move to another node)
// Make sure to cout the "responses" (i.e., list of activities)
void AgentsSimulator::Explore(const InitialAgentData &initial_agent_data) const {
    //Sample code
    std::lock_guard lock(*graphMutexes.at(initial_agent_data.startNode));
    {
        std::lock_guard display(m);
        std::cout << std::this_thread::get_id() << " Working on node " << initial_agent_data.startNode << std::endl;
    }
    std::this_thread::sleep_for(std::chrono::seconds(initial_agent_data.y));
    {
        std::lock_guard display(m);
        std::cout << std::this_thread::get_id() << " Done on node " << initial_agent_data.startNode << std::endl;
    }
}


