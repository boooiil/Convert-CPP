/*********************************************************************
 * @file   RegexUtils.h
 * @brief  Regex utilities header file.
 *
 * @author boooiil
 * @date   January 2024
 *********************************************************************/

#ifndef REGEX_UTILS
#define REGEX_UTILS

#include <regex>
#include <string>
#include <vector>

 /**
  * @brief Handle various functions that utilize std regex.
  */
class RegexUtils {
public:
  /**
   * @brief Checks if a string matches a regex pattern
   *
   * @param[in] str     - string to be matched
   * @param[in] pattern - regex pattern
   * @return true - if the string matches the pattern
   */
  static bool isMatch(std::string str, std::string pattern);

  /**
   * @brief Checks if a string matches a regex pattern
   *
   * @param[in] str     - string to be matched
   * @param[in] pattern - regex pattern
   * @param[in] flag    - regex flag
   *
   * @return true - if the string matches the pattern
   */
  static bool isMatch(std::string str, std::string pattern,
    std::regex_constants::syntax_option_type flag);

  static bool isMatch(char c, std::string pattern);
  static bool isMatch(char* c, std::string pattern);

  static bool isMatch(char c, std::string pattern, std::regex_constants::syntax_option_type flag);
  static bool isMatch(char* c, std::string pattern, std::regex_constants::syntax_option_type flag);

  /**
   * @brief Get the first match of a string that matches a regex pattern
   *
   * @param[in] str     - string to be matched
   * @param[in] pattern - regex pattern
   *
   * @return std::string - first match of the string
   */
  static std::string getFirstMatch(std::string str, std::string pattern);

  /**
   * @brief Get the first match of a string that matches a regex pattern
   *
   * @param[in] str     - string to be matched
   * @param[in] pattern - regex pattern
   * @param[in] flag    - regex flag
   *
   * @return std::string - first match of the string
   */
  static std::string getFirstMatch(
    std::string str, std::string pattern,
    std::regex_constants::syntax_option_type flag);

  /**
   * @brief Get all matches of a string that matches a regex pattern
   *
   * @param[in] str     - string to be matched
   * @param[in] pattern - regex pattern
   *
   * @return All matches of the string
   */
  static std::vector<std::string> getAllMatches(std::string str,
    std::string pattern);

  /**
   * @brief Get all matches of a string that matches a regex pattern
   *
   * @param[in] str     - string to be matched
   * @param[in] pattern - regex pattern
   * @param[in] flag    - regex flag
   *
   * @return All matches of the string
   */
  static std::vector<std::string> getAllMatches(
    std::string str, std::string pattern,
    std::regex_constants::syntax_option_type flag);

  static std::regex createRegex(std::string pattern, std::regex_constants::syntax_option_type flag);
};

#endif  // !REGEX_UTILS
