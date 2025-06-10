/*********************************************************************
 * @file   StringUtils.h
 * @brief  String utilities header file.
 *
 * @author boooiil
 * @date   January 2024
 *********************************************************************/

#ifndef STRING_UTILS
#define STRING_UTILS

#include <regex>
#include <string>
#include <vector>

/**
 * @brief Handle various functions that modify std strings.
 */
class StringUtils {
 public:
  /**
   * @brief Truncate a string.
   *
   * @param[in] str     - The string to truncate.
   * @param[in] length  - The length to truncate to.
   * @return The truncated string.
   */
  static std::string truncateString(std::string str, int length);

  /**
   * @brief Replace all occurances of a sequence of characters with a provided
   * sequence of characters.
   *
   * @param[in] str   - The string to modify.
   * @param[in] from  - The sequence of characters to replace.
   * @param[in] to    -  The sequence of characters to replace with.
   * @return The modified string.
   */
  static std::string replaceAll(std::string str, std::string from,
                                std::string to);

  /**
   * @brief Replace all occurances of a regex pattern with a provided sequence
   * of characters.
   *
   * @param[in] str   - The string to modify.
   * @param[in] from  - The regex pattern to match.
   * @param[in] to    - The sequence of characters to replace with.
   * @return The modified string.
   */
  static std::string replaceAll(std::string str, std::regex from,
                                std::string to);

  /**
   * @brief Turn all characters to lower case.
   *
   * @param[in] str - The string to modify.
   * @return The modified string.
   */
  static std::string toLowerCase(std::string str);

  /**
   * @brief Turn all characters to upper case.
   *
   * @param[in] str - The string to modify.
   * @return The modified string.
   */
  static std::string toUpperCase(std::string str);

  /**
   * @brief Check if a string contains another.
   *
   * @param str The string to check.
   * @param pattern The pattern to match.
   * @return true
   * @return false
   */
  static bool contains(std::string str, std::string pattern);
};

#endif  // !STRING_UTILS
