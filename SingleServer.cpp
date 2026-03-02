#include "SingleServer.h"

/**
 * @file SingleServer.cpp
 * @brief Implements server request processing.
 */

SingleServer::SingleServer(int id) : serverId(id), completedCount(0), hasActive(false), active{0, "", "", 0, 0} {}

bool SingleServer::idle() const {
    return !hasActive;
}

int SingleServer::id() const {
    return serverId;
}

int SingleServer::completed() const {
    return completedCount;
}

void SingleServer::assign(const Request& request) {
    active = request;
    active.jobType = 1;
    hasActive = true;
}

void SingleServer::tick() {
    if (!hasActive) {
        return;
    }

    active.time -= 1;
    if (active.time <= 0) {
        completedCount += 1;
        hasActive = false;
        active = {0, "", "", 0, 0};
    }
}

int SingleServer::timeLeft() const {
    if (!hasActive) {
        return 0;
    }
    return active.time;
}

int SingleServer::taskNumber() const {
    if (!hasActive) {
        return 0;
    }
    return active.taskNumber;
}
