#include "checkkey.hpp"
#include "readconfig.hpp"
#include <cctype>
#include <iostream>

bool isAccessKeyValid() {
    UnScreenSaverConfig config = getConfigFromYaml();
    const std::string& key = config.accessKey;

    // Print the key for debugging
    // std::cout << "Access Key: " << key << std::endl;

    // Check if key is not "1234567890"
    if (key == "1234567890") {
        std::cout << "Key is '1234567890', which is invalid." << std::endl;
        return false;
    }

    // Check if the key has a minimum of 44 characters
    if (key.length() < 40) {
        std::cout << "Key length is less than 44 characters." << std::endl;
        return false;
    }

    // Check if the key has UPPER / lower | numbers
    bool hasUpper = false, hasLower = false, hasDigit = false;
    for (char ch : key) {
        if (std::isupper(ch)) hasUpper = true;
        if (std::islower(ch)) hasLower = true;
        if (std::isdigit(ch)) hasDigit = true;
    }

    // Ensure the key has at least one uppercase letter, one lowercase letter, and one digit
    if (!hasUpper || !hasLower || !hasDigit) {
        std::cout << "Key does not meet the required character criteria." << std::endl;
        return false;
    }

    return true;
}