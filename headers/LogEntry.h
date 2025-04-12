//
// Created by Routscape on 12 Apr 2025.
//

#ifndef LOGENTRY_H
#define LOGENTRY_H
struct LogEntry {
    std::chrono::system_clock::time_point timestamp;
    std::string agent;
    std::string state;    // "is waiting for", "is at", or "exited"
    std::string nodeName;
};
#endif //LOGENTRY_H
