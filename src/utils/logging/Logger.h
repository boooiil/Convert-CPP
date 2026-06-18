#ifndef LOGGER_H
#define LOGGER_H

#include "LogColor.h"
#include <filesystem>
#include <iostream>
#include <sstream>
#include <string>

// Macro to automatically capture the file and line number
#define LOG(priority, ...) Logger::log(priority, __VA_ARGS__)
#define LOG_DEBUG(priority, ...)                                               \
  Logger::debug(__FILE__, __LINE__, priority, __VA_ARGS__)
#define LOG_VERBOSE(priority, ...)                                             \
  Logger::verbose(__FILE__, __LINE__, priority, __VA_ARGS__)

class Logger {
public:
  static bool debug_flag;
  static bool verbose_flag;

  enum Priority { INFO, WARNING, ERROR };

  // Variadic template function to handle different types of arguments
  template <typename... Args> static void log(Priority priority, Args... args) {
    std::ostringstream oss;
    logToStream(oss, args...);
    std::string output = oss.str();
    std::cout << priorityHandler(priority, output) << std::endl;
  }

  template <typename... Args>
  static void debug(const char *file, int line, Priority priority,
                    Args... args) {
    if (debug_flag) {
      std::string fileName = getFileName(file);
      std::string line_str = toString(line);
      log(priority, linePrefix(file, line), args...);
    }
  }

  template <typename... Args>
  static void verbose(const char *file, int line, Priority priority,
                      Args... args) {
    if (verbose_flag) {
      std::string fileName = getFileName(file);
      std::string line_str = toString(line);
      log(priority, linePrefix(file, line), args...);
    }
  }

private:
  static std::string priorityHandler(Priority priority, std::string &str) {
    switch (priority) {
    case ERROR:
      return LogColor::fgRed("[ERROR] " + str);
      break;
    case WARNING:
      return LogColor::fgYellow("[WARNING] " + str);
      break;
    case INFO:
    default:
      return "[INFO] " + str;
    }
  }

  static std::string linePrefix(const char *file, int line) {
    return '[' + getFileName(file) + ':' + toString(line) + ']';
  }

  // Recursive function to log multiple arguments
  template <typename First, typename... Rest>
  static void logToStream(std::ostream &stream, const First &first,
                          const Rest &...rest) {
    stream << toString(first) << " "; // Convert the first argument to string
    logToStream(stream, rest...);     // Recurse for the remaining arguments
  }

  // Helper function to extract the file name from the path
  static std::string getFileName(const char *filePath) {
    std::filesystem::path path(filePath);
    return path.filename().string(); // Get just the file name
  }

  // Helper function to convert various types to string
  template <typename T> static std::string toString(const T &arg) {
    std::ostringstream oss;
    oss << arg;
    return oss.str();
  }

  // Base case: when there are no arguments left
  static void logToStream(std::ostream &stream) {
    // No-op base case for recursion
    (void)stream;
  }
};

#endif // LOGGER_H
