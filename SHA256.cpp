#include "SHA256.h"
#include <sstream>
#include <iomanip>
#include <cstdint>

std::string SHA256::hash(const std::string &data) {
    uint64_t hash = 5381;  // Starting with a large prime seed value
    for (size_t i = 0; i < data.size(); ++i) {
        // Mix data with a prime multiplier, shifting, and XOR
        hash = ((hash << 5) + hash) + static_cast<unsigned char>(data[i]);
        hash ^= (data[i] * 31 + i); // Use XOR with a constant and position
    }

    // Convert the hash to a hex string
    std::stringstream ss;
    ss << std::hex << std::setw(16) << std::setfill('0') << hash;
    return ss.str();
}

