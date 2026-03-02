#include "Request.h"

#include <cstdlib>

/**
 * @file Request.cpp
 * @brief Implements random request generation.
 */

Request RequestBuilder::randomRequest(int minTime, int maxTime) {
    Request request;
    request.taskNumber = 0;
    request.ipIn = randomIp();
    request.ipOut = randomIp();
    request.time = minTime + (std::rand() % (maxTime - minTime + 1));
    request.jobType = 0;
    return request;
}

std::string RequestBuilder::randomIp() {
    return std::to_string((std::rand() % 254) + 1) + "." +
           std::to_string((std::rand() % 254) + 1) + "." +
           std::to_string((std::rand() % 254) + 1) + "." +
           std::to_string((std::rand() % 254) + 1);
}
