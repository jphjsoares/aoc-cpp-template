#include "common/utils.h"
#include <algorithm>
#include <stdexcept>

namespace aoc {
namespace utils {

std::string readFile(const std::string& filepath) {
    std::ifstream file(filepath);
    if (!file.is_open()) {
        throw std::runtime_error("Failed to open file: " + filepath);
    }

    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

std::vector<std::string> split(const std::string& str, char delimiter) {
    std::vector<std::string> tokens;
    std::string token;
    std::stringstream ss(str);

    while (std::getline(ss, token, delimiter)) {
        tokens.push_back(token);
    }

    return tokens;
}

std::vector<std::string> splitWhitespace(const std::string& str) {
    std::vector<std::string> tokens;
    std::string token;
    std::stringstream ss(str);

    while (ss >> token) {
        tokens.push_back(token);
    }

    return tokens;
}

std::vector<std::string> readLines(const std::string& filepath) {
    std::ifstream file(filepath);
    if (!file.is_open()) {
        throw std::runtime_error("Failed to open file: " + filepath);
    }

    std::vector<std::string> lines;
    std::string line;

    while (std::getline(file, line)) {
        lines.push_back(line);
    }

    return lines;
}

std::string trim(const std::string& str) {
    const std::string whitespace = " \t\n\r\f\v";

    size_t start = str.find_first_not_of(whitespace);
    if (start == std::string::npos) {
        return "";
    }

    size_t end = str.find_last_not_of(whitespace);
    return str.substr(start, end - start + 1);
}

int toInt(const std::string& str) {
    return std::stoi(str);
}

long toLong(const std::string& str) {
    return std::stol(str);
}

long long toLongLong(const std::string& str) {
    return std::stoll(str);
}

} // namespace utils
} // namespace aoc
