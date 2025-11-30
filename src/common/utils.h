#ifndef AOC_UTILS_H
#define AOC_UTILS_H

#include <fstream>
#include <sstream>
#include <string>
#include <vector>

namespace aoc {
namespace utils {

/**
 * Read entire file contents into a string
 */
std::string readFile(const std::string& filepath);

/**
 * Split a string by delimiter
 */
std::vector<std::string> split(const std::string& str, char delimiter);

/**
 * Split a string by any whitespace
 */
std::vector<std::string> splitWhitespace(const std::string& str);

/**
 * Read file as lines
 */
std::vector<std::string> readLines(const std::string& filepath);

/**
 * Trim whitespace from both ends of string
 */
std::string trim(const std::string& str);

/**
 * Convert string to integer
 */
int toInt(const std::string& str);

/**
 * Convert string to long
 */
long toLong(const std::string& str);

/**
 * Convert string to long long
 */
long long toLongLong(const std::string& str);

}  // namespace utils
}  // namespace aoc

#endif  // AOC_UTILS_H
