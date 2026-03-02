#include <iostream>

#include "LoadBalancer.h"
#include "Request.h"

/**
 * @file main.cpp
 * @brief Runs the load balancer simulation.
 */

int main() {
    int initialServers;
    int clockCycles;

    std::cout << "Initial Servers: ";
    std::cin >> initialServers;
    std::cout << "Clock Cycles: ";
    std::cin >> clockCycles;

    LoadBalancer balancer(initialServers, clockCycles);
    balancer.startingQueueCalc();
    balancer.run();

    return 0;
}
