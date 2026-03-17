#include "ListUtils.h"

#include <cstring>
#include <regex>
#include <string>
#include <vector>

std::string ListUtils::join(std::vector<std::string> list, std::string delim) {
  std::string result = "";
  for (int i = 0; i < list.size(); i++) {
    result += list[i];
    if (i < list.size() - 1) {
      result += delim;
    }
  }
  return result;
}

std::vector<std::string> ListUtils::toStrVector(char* arr[]) {
  std::vector<std::string> result;
  for (int i = 0; arr[i] != nullptr; i++) {
    result.push_back(arr[i]);
  }
  return result;
}

std::vector<char*> ListUtils::toCharVector(char* arr[]) {
  std::vector<char*> result;
  for (int i = 0; arr[i] != nullptr; i++) {
    result.push_back(arr[i]);
  }
  return result;
}

std::vector<char> ListUtils::toCharVector(char arr[]) {
  std::vector<char> result;
  for (int i = 0; arr[i] != '\0'; i++) {
    result.push_back(arr[i]);
  }
  return result;
}

char** ListUtils::toCharArray(std::vector<std::string> list) {
  char** result = new char* [list.size() + 1];
  for (int i = 0; i < list.size(); i++) {
    result[i] = new char[list[i].size() + 1];
    strcpy(result[i], list[i].c_str());
  }
  result[list.size()] = nullptr;
  return result;
}

std::vector<std::string> ListUtils::splitv(std::string str, std::string delim) {
  std::vector<std::string> result;
  std::string token;
  size_t pos = 0;
  while ((pos = str.find(delim)) != std::string::npos) {
    token = str.substr(0, pos);
    result.push_back(token);
    str.erase(0, pos + delim.length());
  }
  result.push_back(str);
  return result;
}

std::vector<std::string> ListUtils::splitv(std::string str, std::regex delim) {
  // iterate through each char, building the str until it reaches the match
  std::string built;
  std::vector<std::string> result;
  std::smatch match;

  for (char c : str) {
    std::string c_str(1, c);
    if (std::regex_match(c_str, match, delim)) {
      result.push_back(built);
      built = "";
      continue;
    }
    else {
      built += c;
    }
  }

  return result;
}
