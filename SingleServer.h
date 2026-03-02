#ifndef SINGLESERVER_H
#define SINGLESERVER_H

#include "Request.h"

/**
 * @file SingleServer.h
 * @brief Defines the web server worker used by the load balancer.
 */

/**
 * @class SingleServer
 * @brief Processes one request at a time.
 */
class SingleServer {
public:
    /**
     * @brief Creates a server with an ID number.
     * @param id Server ID.
     */
    explicit SingleServer(int id);

    /**
     * @brief Checks whether the server is idle.
     * @return True if no active request is assigned.
     */
    bool idle() const;

    /**
     * @brief Gets the server ID.
     * @return Server ID.
     */
    int id() const;

    /**
     * @brief Gets how many requests this server has completed.
     * @return Completed request count.
     */
    int completed() const;

    /**
     * @brief Assigns a request to this server.
     * @param request Request to process.
     */
    void assign(const Request& request);

    /**
     * @brief Advances the active request by one cycle.
     */
    void tick();

    /**
     * @brief Gets the remaining time on the active task.
     * @return Remaining cycles, or 0 if idle.
     */
    int timeLeft() const;

    /**
     * @brief Gets the active task number.
     * @return Task number, or 0 if idle.
     */
    int taskNumber() const;

private:
    int serverId;
    int completedCount;
    bool hasActive;
    Request active;
};

#endif
