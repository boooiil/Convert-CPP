/*********************************************************************
 * @file   ListUtils.h
 * @brief  List utilities header file.
 *
 * @author boooiil
 * @date   January 2024
 *********************************************************************/

#ifndef LIST_UTILS_H
#define LIST_UTILS_H

#include <string>
#include <vector>

/**
 * @brief Handle various functions that involve list-like data structures.
 */
class ListUtils {
 public:
  /**
   * @brief Join a list of strings.
   *
   * @param[in] list  - The list of strings.
   * @param[in] delim - The delimiter to join the strings with.
   * @return The joined string.
   */
  static std::string join(std::vector<std::string> list,
                          std::string delim = "");

  /**
   * @brief Split a string into a vector of strings.
   *
   * @param[in] str   - The string to split.
   * @param[in] delim - The delimiter to split the string with.
   * @return A list of strings.
   */
  static std::vector<std::string> splitv(std::string str,
                                         std::string delim = "");
};

#endif