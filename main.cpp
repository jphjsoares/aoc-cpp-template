#include <chrono>
#include <cstring>
#include <functional>
#include <iomanip>
#include <iostream>
#include <string>
#include <vector>

#include "src/common/input_fetcher.h"
#include "src/common/utils.h"

// Include day solutions
#include "src/2025/day_1/solution.h"
// ... add more as you implement them

struct DayResult {
  int year;
  int day;
  int part;
  std::string answer;
  double timeMs;
};

struct Config {
  int year = 2025;  // Default year
  int day = -1;     // -1 means all days
  int part = -1;    // -1 means both parts
  bool showHelp = false;
};

void printHelp(const char* programName) {
  std::cout << "Advent of Code C++ Solution Runner\n\n";
  std::cout << "Usage: " << programName << " [options]\n\n";
  std::cout << "Options:\n";
  std::cout << "  -y, --year YEAR    Specify year (default: 2025)\n";
  std::cout << "  -d, --day DAY      Specify day (1-25, default: all days)\n";
  std::cout << "  -p, --part PART    Specify part (1-2, default: both parts)\n";
  std::cout << "  -h, --help         Show this help message\n\n";
  std::cout << "Examples:\n";
  std::cout << "  " << programName
            << "                          # Run all days of 2025\n";
  std::cout << "  " << programName
            << " -y 2025 -d 1             # Run day 1 of 2025\n";
  std::cout << "  " << programName
            << " --year 2024 --day 5 -p 2 # Run 2024 day 5 part 2\n";
  std::cout
      << "  " << programName
      << " -d 10                    # Run day 10 of 2025 (default year)\n";
}

Config parseArgs(int argc, char* argv[]) {
  Config config;

  for (int i = 1; i < argc; i++) {
    std::string arg = argv[i];

    if (arg == "-h" || arg == "--help") {
      config.showHelp = true;
      return config;
    } else if (arg == "-y" || arg == "--year") {
      if (i + 1 < argc) {
        config.year = std::stoi(argv[++i]);
      } else {
        std::cerr << "Error: " << arg << " requires an argument\n";
        config.showHelp = true;
        return config;
      }
    } else if (arg == "-d" || arg == "--day") {
      if (i + 1 < argc) {
        config.day = std::stoi(argv[++i]);
      } else {
        std::cerr << "Error: " << arg << " requires an argument\n";
        config.showHelp = true;
        return config;
      }
    } else if (arg == "-p" || arg == "--part") {
      if (i + 1 < argc) {
        config.part = std::stoi(argv[++i]);
      } else {
        std::cerr << "Error: " << arg << " requires an argument\n";
        config.showHelp = true;
        return config;
      }
    } else {
      std::cerr << "Error: Unknown option: " << arg << "\n";
      config.showHelp = true;
      return config;
    }
  }

  return config;
}

template <typename Func>
DayResult runPart(int year, int day, int part, const std::string& input,
                  Func func) {
  auto start = std::chrono::high_resolution_clock::now();
  std::string answer = func(input);
  auto end = std::chrono::high_resolution_clock::now();

  std::chrono::duration<double, std::milli> duration = end - start;

  return {year, day, part, answer, duration.count()};
}

void printResults(const std::vector<DayResult>& results) {
  std::cout << "\n";
  std::cout << "+=======+======+======+==================+===============+\n";
  std::cout << "| Year  | Day  | Part |      Answer      |   Time (ms)   |\n";
  std::cout << "+=======+======+======+==================+===============+\n";

  for (const auto& result : results) {
    std::cout << "| " << std::setw(5) << result.year << " ";
    std::cout << "| " << std::setw(4) << result.day << " ";
    std::cout << "| " << std::setw(4) << result.part << " ";
    std::cout << "| " << std::setw(16) << result.answer << " ";
    std::cout << "| " << std::setw(13) << std::fixed << std::setprecision(3)
              << result.timeMs << " ";
    std::cout << "|\n";
  }

  std::cout << "+=======+======+======+==================+===============+\n";

  // Calculate total time
  double totalTime = 0.0;
  for (const auto& result : results) {
    totalTime += result.timeMs;
  }

  std::cout << "| Total time: " << std::setw(13) << std::fixed
            << std::setprecision(3) << totalTime << " ms";
  std::cout << std::setw(27) << "|\n";
  std::cout << "+=======+======+======+==================+===============+\n";
  std::cout << "\n";
}

int main(int argc, char* argv[]) {
  Config config = parseArgs(argc, argv);

  if (config.showHelp) {
    printHelp(argv[0]);
    return config.showHelp && argc > 1 ? 1 : 0;
  }

  std::vector<DayResult> results;

  // Helper lambda to run a day's solutions
  auto runDay = [&](int year, int day,
                    std::function<std::string(const std::string&)> p1,
                    std::function<std::string(const std::string&)> p2) {
    // Skip if not matching target year/day
    if (config.year != year) {
      return;
    }
    if (config.day != -1 && config.day != day) {
      return;
    }

    std::string inputPath = "src/" + std::to_string(year) + "/day_" +
                            std::to_string(day) + "/tests/input.txt";

    // Try to fetch input if it doesn't exist
    aoc::fetcher::fetchInput(year, day, inputPath);

    // Read input
    std::string input;
    try {
      input = aoc::utils::readFile(inputPath);
    } catch (const std::exception& e) {
      std::cerr << "Warning: Could not read input for year " << year << " day "
                << day << ": " << e.what() << "\n";
      std::cerr << "Skipping year " << year << " day " << day << "\n";
      return;
    }

    // Run parts
    if (config.part == -1 || config.part == 1) {
      results.push_back(runPart(year, day, 1, input, p1));
    }
    if (config.part == -1 || config.part == 2) {
      results.push_back(runPart(year, day, 2, input, p2));
    }
  };

  std::cout << "Advent of Code - C++ Solutions\n";
  std::cout << "================================\n";

  if (config.day != -1) {
    std::cout << "Running Year " << config.year << " Day " << config.day;
    if (config.part != -1) {
      std::cout << " Part " << config.part;
    }
    std::cout << "\n";
  } else {
    std::cout << "Running all available solutions for year " << config.year
              << "\n";
  }

  // Run day solutions for each year
  // Year 2025
  runDay(2025, 1, day_1::part1, day_1::part2);
  // runDay(2025, 2, day_2::part1, day_2::part2);
  // ... add more 2025 days as you implement them

  // Year 2024 (example)
  // runDay(2024, 1, day_1_2024::part1, day_1_2024::part2);
  // ... add more years/days as needed

  // Print results
  if (results.empty()) {
    std::cout
        << "\nNo solutions were run. Check your input files or filters.\n";
  } else {
    printResults(results);
  }

  return 0;
}
