#ifndef LOGGER_H
#define LOGGER_H

#include <filesystem>
#include <iostream>
#include <sstream>
#include <string>

// Macro to automatically capture the file and line number
#define LOG(...) Logger::log(__VA_ARGS__)
#define LOG_DEBUG(...) Logger::debug(__FILE__, __LINE__, __VA_ARGS__)

class Logger {
 public:
  static bool debug_flag;

  // Helper function to extract the file name from the path
  static std::string getFileName(const char* filePath) {
    std::filesystem::path path(filePath);
    return path.filename().string();  // Get just the file name
  }

  // Helper function to convert various types to string
  template <typename T>
  static std::string toString(const T& arg) {
    std::ostringstream oss;
    oss << arg;
    return oss.str();
  }

  // Base case: when there are no arguments left
  static void logToStream(std::ostream& stream) {
    // No-op base case for recursion
      (void)stream;
  }

  // Recursive function to log multiple arguments
  template <typename First, typename... Rest>
  static void logToStream(std::ostream& stream, const First& first,
                          const Rest&... rest) {
    stream << toString(first) << " ";  // Convert the first argument to string
    logToStream(stream, rest...);      // Recurse for the remaining arguments
  }

  // Variadic template function to handle different types of arguments
  template <typename... Args>
  static void log(Args... args) {
    std::ostringstream oss;
    logToStream(oss, args...);
    std::cout << oss.str() << std::endl;
  }

  template <typename... Args>
  static void debug(const char* file, int line, Args... args) {
    if (debug_flag) {
      std::string fileName = getFileName(file);
      std::string line_str = toString(line);
      log("[" + fileName + ":" + line_str + "]", args...);
    }
  }
};

#endif  // LOGGER_H
