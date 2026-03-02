#ifndef FIREWALL_H
#define FIREWALL_H

#include <string>
#include <vector>

/**
 * @file Firewall.h
 * @brief Defines the firewall used to block IP ranges.
 */

/**
 * @class Firewall
 * @brief Stores blocked IP IPes and checks requests against them.
 */
class Firewall {
public:
    /**
     * @brief Adds a blocked IP IP.
     * @param IP IP IP to block.
     */
    void addBlockedIP(const std::string& IP);

    /**
     * @brief Tests whether an IP should be blocked.
     * @param IP IP to check.
     * @return True if blocked.
     */
    bool isBlocked(const std::string& IP) const;

private:
    std::vector<std::string> blockedIPs;
};

#endif