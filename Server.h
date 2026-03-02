#ifndef SERVER_H
#define SERVER_H

#include <vector>

#include "SingleServer.h"

/**
 * @file Server.h
 * @brief Defines the server pool container.
 */

/**
 * @class Server
 * @brief Stores and manages a collection of web servers.
 */
class Server {
public:
    /**
     * @brief Creates a server pool.
     * @param count Number of starting web servers.
     */
    explicit Server(int count = 1);

    /**
     * @brief Gets the number of active web servers.
     * @return Server count.
     */
    int count() const;

    /**
     * @brief Adds one server to the pool.
     */
    void addOne();

    /**
     * @brief Removes one server from the pool.
     * @return True if removal happened.
     */
    bool removeOne();

    /**
     * @brief Accesses the worker collection.
     * @return Reference to all web servers.
     */
    std::vector<SingleServer>& workers();

private:
    std::vector<SingleServer> workerList;
};

#endif
