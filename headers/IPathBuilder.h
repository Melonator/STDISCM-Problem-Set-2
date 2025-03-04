#ifndef IPATHBUILDER_H
#define IPATHBUILDER_H
#include <string>

class IPathBuilder {
public:
    virtual void displayPath(const std::string& start, const std::string& end);
};

#endif //IPATHBUILDER_H
