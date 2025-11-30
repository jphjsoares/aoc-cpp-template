#ifndef AOC_INPUT_FETCHER_H
#define AOC_INPUT_FETCHER_H

#include <string>

namespace aoc {
namespace fetcher {

/**
 * Fetch input for a specific day from Advent of Code website
 * Uses session cookie from .env file
 *
 * @param year The year (e.g., 2025)
 * @param day The day (1-25)
 * @param outputPath Where to save the input file
 * @return true if successful, false otherwise
 */
bool fetchInput(int year, int day, const std::string& outputPath);

/**
 * Load session cookie from .env file
 *
 * @return Session cookie value, or empty string if not found
 */
std::string loadSessionCookie();

}  // namespace fetcher
}  // namespace aoc

#endif  // AOC_INPUT_FETCHER_H
