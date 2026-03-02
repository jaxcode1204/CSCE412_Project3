#include "Firewall.h"

/**
 * @file Firewall.cpp
 * @brief Implements firewall IP checks.
 */

void Firewall::addBlockedIP(const std::string& IP) {
    blockedIPs.push_back(IP);
}

bool Firewall::isBlocked(const std::string& IP) const {
    for (const auto& blocked : blockedIPs) {
        if (IP.rfind(blocked, 0) == 0) {
            return true;
        }
    }
    return false;
}