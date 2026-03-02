#ifndef LOADBALANCER_H
#define LOADBALANCER_H

#include <queue>
#include <string>

#include "Firewall.h"
#include "Request.h"
#include "Server.h"

/**
 * @file LoadBalancer.h
 * @brief Defines load balancer behavior.
 */

/**
 * @class LoadBalancer
 * @brief Manages the request queue, time, servers, and scaling rules.
 */
class LoadBalancer {
public:
    LoadBalancer(int initialServers, int clockCycles);

    void addBlockedIP(const std::string& IP);
    void addToQueue(const Request& request);
    void startingQueueCalc();
    void run();

private:
    void loadConfig();
    void randomRequestAdder();
    void goToWork();
    void serverAdjustment();

    Firewall firewall;
    Server serverPool;
    std::queue<Request> requestQueue;
    int initialServers;
    int clockCycles;
    int initialQueueMultiplier;
    int minProcessTime;
    int maxProcessTime;
    int restCycles;
    int lowThresholdPerServer;
    int highThresholdPerServer;
    double newRequestChance;
    int cycle;
    int lastAdjustmentCycle;
    int blockedCount;
    int completedCount;
    int nextTaskNumber;
    int startingQueueSize;
};

#endif
