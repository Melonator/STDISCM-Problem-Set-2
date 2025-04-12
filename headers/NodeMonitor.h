#ifndef NODEMONITOR_H
#define NODEMONITOR_H
#include <condition_variable>
#include <mutex>
#include <queue>

class NodeMonitor {
public:
    bool isAgentTurn(int agent_id);

    void waitForTurn(int agent_id);

    void releaseTurn();
    void addToQueue(int agent_id);
private:
    std::queue<int> accessQueue;
    std::mutex mtx;
    std::condition_variable cv;
};

#endif //NODEMONITOR_H
