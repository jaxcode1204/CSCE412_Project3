#ifndef REQUEST_H
#define REQUEST_H

#include <string>

/**
 * @file Request.h
 * @brief Defines the request data and request builder helpers.
 */

/**
 * @struct Request
 * @brief Stores one request moving through the load balancer.
 */
struct Request {
    int taskNumber;
    std::string ipIn;
    std::string ipOut;
    int time;
    int jobType;
};

/**
 * @struct RequestBuilder
 * @brief Creates randomized requests for the simulation.
 */
struct RequestBuilder {
    /**
     * @brief Builds one randomized request.
     * @param minTime Minimum task time.
     * @param maxTime Maximum task time.
     * @return Randomized request.
     */
    static Request randomRequest(int minTime, int maxTime);

    /**
     * @brief Builds one randomized IPv4 address.
     * @return Random IPv4 address string.
     */
    static std::string randomIp();
};

#endif
