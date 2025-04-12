#include "../headers/NodeMonitor.h"


bool NodeMonitor::isAgentTurn(int agent_id) {
    return accessQueue.front() == agent_id;
}

void NodeMonitor::waitForTurn(int agent_id) {
    std::unique_lock lock(mtx);
    // Directly check the queue here (no separate unsafe call)
    cv.wait(lock, [this, agent_id] {
        return !accessQueue.empty() && accessQueue.front() == agent_id;
    });
}

void NodeMonitor::releaseTurn() {
    std::unique_lock lock(mtx);
    if (!accessQueue.empty()) {
        accessQueue.pop();
    }
    cv.notify_all();
}

void NodeMonitor::addToQueue(int agent_id) {
    accessQueue.push(agent_id);
}
