#include "Server.h"

/**
 * @file Server.cpp
 * @brief Implements the server pool.
 */

Server::Server(int count) {
    if (count < 1) {
        count = 1;
    }

    for (int i = 0; i < count; ++i) {
        workerList.emplace_back(i + 1);
    }
}

int Server::count() const {
    return static_cast<int>(workerList.size());
}

void Server::addOne() {
    workerList.emplace_back(static_cast<int>(workerList.size()) + 1);
}

bool Server::removeOne() {
    if (workerList.size() <= 1) {
        return false;
    }

    workerList.pop_back();
    return true;
}

std::vector<SingleServer>& Server::workers() {
    return workerList;
}
