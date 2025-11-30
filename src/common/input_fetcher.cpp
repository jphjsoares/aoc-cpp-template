#include "common/input_fetcher.h"
#include "common/utils.h"
#include <curl/curl.h>
#include <fstream>
#include <iostream>
#include <sstream>

namespace aoc {
namespace fetcher {

// Callback function for CURL to write data
static size_t writeCallback(void* contents, size_t size, size_t nmemb, void* userp) {
    ((std::string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}

std::string loadSessionCookie() {
    try {
        // Try to read from .env file
        std::ifstream envFile(".env");
        if (!envFile.is_open()) {
            std::cerr << "Warning: .env file not found. Cannot fetch inputs automatically.\n";
            std::cerr << "Copy .env.example to .env and add your session cookie.\n";
            return "";
        }

        std::string line;
        while (std::getline(envFile, line)) {
            // Skip comments and empty lines
            line = utils::trim(line);
            if (line.empty() || line[0] == '#') {
                continue;
            }

            // Look for AOC_SESSION=value
            if (line.find("AOC_SESSION=") == 0) {
                std::string value = line.substr(12); // Skip "AOC_SESSION="
                value = utils::trim(value);
                if (value.empty() || value == "your_session_cookie_here") {
                    std::cerr << "Warning: AOC_SESSION not configured in .env file.\n";
                    return "";
                }
                return value;
            }
        }

        std::cerr << "Warning: AOC_SESSION not found in .env file.\n";
        return "";
    } catch (const std::exception& e) {
        std::cerr << "Error reading .env file: " << e.what() << "\n";
        return "";
    }
}

bool fetchInput(int year, int day, const std::string& outputPath) {
    // Check if input already exists
    std::ifstream existingFile(outputPath);
    if (existingFile.good()) {
        std::cout << "Input file already exists: " << outputPath << "\n";
        return true;
    }

    // Load session cookie
    std::string sessionCookie = loadSessionCookie();
    if (sessionCookie.empty()) {
        std::cerr << "Cannot fetch input: No valid session cookie.\n";
        return false;
    }

    // Initialize CURL
    CURL* curl = curl_easy_init();
    if (!curl) {
        std::cerr << "Failed to initialize CURL\n";
        return false;
    }

    // Build URL
    std::stringstream urlStream;
    urlStream << "https://adventofcode.com/" << year << "/day/" << day << "/input";
    std::string url = urlStream.str();

    // Build cookie header
    std::string cookie = "session=" + sessionCookie;

    std::string responseData;

    // Set CURL options
    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_COOKIE, cookie.c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &responseData);
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);

    // Perform request
    CURLcode res = curl_easy_perform(curl);

    // Check for errors
    if (res != CURLE_OK) {
        std::cerr << "CURL request failed: " << curl_easy_strerror(res) << "\n";
        curl_easy_cleanup(curl);
        return false;
    }

    // Check HTTP response code
    long httpCode = 0;
    curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &httpCode);
    curl_easy_cleanup(curl);

    if (httpCode != 200) {
        std::cerr << "HTTP request failed with code: " << httpCode << "\n";
        if (httpCode == 404) {
            std::cerr << "Day " << day << " input not yet available.\n";
        } else if (httpCode == 400 || httpCode == 500) {
            std::cerr << "Invalid session cookie or server error.\n";
        }
        return false;
    }

    // Save to file
    std::ofstream outFile(outputPath);
    if (!outFile.is_open()) {
        std::cerr << "Failed to open output file: " << outputPath << "\n";
        return false;
    }

    outFile << responseData;
    outFile.close();

    std::cout << "Successfully fetched input for day " << day << " -> " << outputPath << "\n";
    return true;
}

} // namespace fetcher
} // namespace aoc
