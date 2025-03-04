#include <iostream>

#include "headers/Pathfinder.h"
#include "headers/sequential/AStarSequential.h"
#include "headers/sequential/BFSSequential.h"

int main() {
    Pathfinder pf;
    auto* as = new AStarSequential();
    pf.setStrategy(as);
    pf.displayPath("a", "b");
    std::cout << "\n";

    auto* bs = new BFSSequential();
    pf.setStrategy(bs);
    pf.displayPath("a", "b");
    return 0;
}
