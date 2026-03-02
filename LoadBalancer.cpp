#include "LoadBalancer.h"

#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>

/**
 * @file LoadBalancer.cpp
 * @brief Implements queue processing, scaling, and logging.
 */

LoadBalancer::LoadBalancer(int initialServers, int clockCycles)
    : firewall(), serverPool(initialServers), requestQueue(), initialServers(initialServers), clockCycles(clockCycles),
      initialQueueMultiplier(100), minProcessTime(1), maxProcessTime(10), restCycles(50), lowThresholdPerServer(50),
      highThresholdPerServer(80), newRequestChance(0.50), cycle(0), lastAdjustmentCycle(0), blockedCount(0),
      completedCount(0), nextTaskNumber(1), startingQueueSize(0) {
    loadConfig();
}

void LoadBalancer::loadConfig() {
    std::ifstream configFile("config");
    if (!configFile) {
        return;
    }

    std::string line;
    while (std::getline(configFile, line)) {
        std::size_t equals = line.find('=');
        if (equals == std::string::npos) {
            continue;
        }

        std::string key = line.substr(0, equals);
        std::string value = line.substr(equals + 1);

        if (key == "initialQueueMultiplier") initialQueueMultiplier = std::stoi(value);
        else if (key == "minProcessTime") minProcessTime = std::stoi(value);
        else if (key == "maxProcessTime") maxProcessTime = std::stoi(value);
        else if (key == "restCycles") restCycles = std::stoi(value);
        else if (key == "lowThresholdPerServer") lowThresholdPerServer = std::stoi(value);
        else if (key == "highThresholdPerServer") highThresholdPerServer = std::stoi(value);
        else if (key == "newRequestChance") newRequestChance = std::stod(value);
        else if (key == "blockedIP") addBlockedIP(value);
    }
}

void LoadBalancer::addBlockedIP(const std::string& IP) {
    firewall.addBlockedIP(IP);
}

void LoadBalancer::addToQueue(const Request& request) {
    Request numberedRequest = request;
    numberedRequest.taskNumber = nextTaskNumber;
    numberedRequest.jobType = 0;
    nextTaskNumber += 1;

    if (firewall.isBlocked(numberedRequest.ipIn)) {
        blockedCount += 1;
        return;
    }

    requestQueue.push(numberedRequest);
}

void LoadBalancer::startingQueueCalc() {
    int total = initialServers * initialQueueMultiplier;
    for (int i = 0; i < total; ++i) {
        addToQueue(RequestBuilder::randomRequest(minProcessTime, maxProcessTime));
    }
    startingQueueSize = static_cast<int>(requestQueue.size());
}

void LoadBalancer::randomRequestAdder() {
    int attempts = serverPool.count();
    if (attempts < 1) {
        attempts = 1;
    }

    for (int i = 0; i < attempts; ++i) {
        double chance = static_cast<double>(std::rand()) / RAND_MAX;
        if (chance <= newRequestChance) {
            addToQueue(RequestBuilder::randomRequest(minProcessTime, maxProcessTime));
        }
    }
}

void LoadBalancer::goToWork() {
    for (auto& worker : serverPool.workers()) {
        if (worker.idle() && !requestQueue.empty()) {
            worker.assign(requestQueue.front());
            requestQueue.pop();
        }

        int before = worker.completed();
        worker.tick();
        int after = worker.completed();

        if (after > before) {
            completedCount += (after - before);
        }
    }
}

void LoadBalancer::serverAdjustment() {
    if (cycle - lastAdjustmentCycle < restCycles) {
        return;
    }

    int serverCount = serverPool.count();
    int queueCount = static_cast<int>(requestQueue.size());
    int lowLimit = lowThresholdPerServer * serverCount;
    int highLimit = highThresholdPerServer * serverCount;

    if (queueCount < lowLimit) {
        if (serverPool.removeOne()) {
            lastAdjustmentCycle = cycle;
            std::cout << "\033[31mRemoved 1 server at cycle " << cycle << "\033[0m\n";
        }
    } else if (queueCount > highLimit) {
        serverPool.addOne();
        lastAdjustmentCycle = cycle;
        std::cout << "\033[32mAdded 1 server at cycle " << cycle << "\033[0m\n";
    }
}

void LoadBalancer::run() {
    std::ofstream logFile("simulation.log");

    if (logFile) {
        logFile << "Initial Servers: " << initialServers << '\n';
        logFile << "Clock Cycles: " << clockCycles << '\n';
        logFile << '\n';
    }

    for (cycle = 1; cycle <= clockCycles; ++cycle) {
        randomRequestAdder();
        goToWork();
        serverAdjustment();

        std::string header =
            "Cycle=" + std::to_string(cycle) +
            " Queue=" + std::to_string(requestQueue.size()) +
            " Servers=" + std::to_string(serverPool.count()) +
            " Completed=" + std::to_string(completedCount) +
            " Blocked=" + std::to_string(blockedCount);

        std::cout << "\033[33m" << header << "\033[0m\n";
        if (logFile) {
            logFile << header << '\n';
        }

        for (auto& worker : serverPool.workers()) {
            std::string serverLine;
            if (worker.idle()) {
                serverLine =
                    "Server " + std::to_string(worker.id()) +
                    " | Task=IDLE" +
                    " | TimeLeft=0";
            } else {
                serverLine =
                    "Server " + std::to_string(worker.id()) +
                    " | Task=" + std::to_string(worker.taskNumber()) +
                    " | TimeLeft=" + std::to_string(worker.timeLeft());
            }

            std::cout << serverLine << '\n';
            if (logFile) {
                logFile << serverLine << '\n';
            }
        }

        std::cout << '\n';
        if (logFile) {
            logFile << '\n';
        }
    }

    int endingQueueSize = static_cast<int>(requestQueue.size());
    int activeServers = 0;
    int inactiveServers = 0;

    for (auto& worker : serverPool.workers()) {
        if (worker.idle()) {
            inactiveServers += 1;
        } else {
            activeServers += 1;
        }
    }

    std::string summary =
        "Summary\n"
        "Starting Queue Size: " + std::to_string(startingQueueSize) + "\n" +
        "Ending Queue Size: " + std::to_string(endingQueueSize) + "\n" +
        "Task Time Range: " + std::to_string(minProcessTime) + " - " + std::to_string(maxProcessTime) + "\n" +
        "Initial Servers: " + std::to_string(initialServers) + "\n" +
        "Final Servers: " + std::to_string(serverPool.count()) + "\n" +
        "Completed Requests: " + std::to_string(completedCount) + "\n" +
        "Rejected Requests: " + std::to_string(blockedCount);

    std::string endStatus =
        "End Status\n"
        "Remaining Requests In Queue: " + std::to_string(endingQueueSize) + "\n" +
        "Active Servers: " + std::to_string(activeServers) + "\n" +
        "Inactive Servers: " + std::to_string(inactiveServers) + "\n" +
        "Rejected Requests: " + std::to_string(blockedCount) + "\n" +
        "Total Servers: " + std::to_string(serverPool.count());

    std::cout << "\033[36m" << summary << "\033[0m\n\n";
    std::cout << "\033[36m" << endStatus << "\033[0m\n";
    if (logFile) {
        logFile << summary << '\n' << '\n';
        logFile << endStatus << '\n';
    }
}
