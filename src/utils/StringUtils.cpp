#include "StringUtils.h"

#include <cmath>
#include <regex>
#include <sstream>
#include <string>

auto StringUtils::truncateString(std::string str, int length = 25)
    -> std::string {
  const int MAX_LENGTH = length;
  if (str.length() <= MAX_LENGTH) {
    return str;
  }

  const std::string ellipsis = "... ";

  const int leftHalfLength =
      static_cast<int>(ceil((MAX_LENGTH - ellipsis.length()) / 2.0));
  const int rightHalfLength =
      static_cast<int>(floor((MAX_LENGTH - ellipsis.length()) / 2.0));

  const std::string leftHalf = str.substr(0, leftHalfLength);
  const std::string rightHalf = str.substr(str.length() - rightHalfLength);

  return leftHalf + ellipsis + rightHalf;
}

auto StringUtils::replaceAll(std::string str, const std::string& from,
                             const std::string& to) -> std::string {
  size_t start_pos = 0;
  while ((start_pos = str.find(from, start_pos)) != std::string::npos) {
    str.replace(start_pos, from.length(), to);
    start_pos +=
        to.length();  // Handles case where 'to' is a substring of 'from'
  }

  return str;
}

auto StringUtils::replaceAll(const std::string& str, const std::regex& from,
                             const std::string& to) -> std::string {
  return std::regex_replace(str, from, to);
}

auto StringUtils::toLowerCase(std::string str) -> std::string {
  // amount to convert from lower <-> upper
  const int x = 32;

  for (char& c : str) {
    if (c >= 65 && c <= 90) {
      c = static_cast<char>(c + x);
    } else {
      c = c;
    };
  }

  return str;
}

auto StringUtils::toUpperCase(std::string str) -> std::string {
  // amount to convert from lower <-> upper
  const int x = 32;

  for (char& c : str) {
    if (c >= 65 && c <= 90) {
      c = static_cast<char>(c - x);
    } else {
      c = c;
    };
  }

  return str;
}

auto StringUtils::contains(const std::string& str, const std::string& pattern)
    -> bool {
  return str.find(pattern) != std::string::npos;
}