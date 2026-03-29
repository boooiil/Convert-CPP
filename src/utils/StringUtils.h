/*********************************************************************
 * @file   StringUtils.h
 * @brief  String utilities header file.
 *
 * @author boooiil
 * @date   January 2024
 *********************************************************************/

#ifndef STRING_UTILS
#define STRING_UTILS

#include "logging/LogColor.h"
#include <regex>
#include <string>

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
  static std::string replaceAll(std::string str, const std::string &from,
                                const std::string &to);

  /**
   * @brief Replace all occurances of a regex pattern with a provided sequence
   * of characters.
   *
   * @param[in] str   - The string to modify.
   * @param[in] from  - The regex pattern to match.
   * @param[in] to    - The sequence of characters to replace with.
   * @return The modified string.
   */
  static std::string replaceAll(const std::string &str, const std::regex &from,
                                const std::string &to);

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
   * @brief Convert a string to proper case (title case).
   *
   * @param[in] str The string to convert.
   * @return The converted string.
   */
  static std::string toProperCase(std::string str);

  /**
   * @brief Wrap content in brackets without changing the inner and outer
   * colors.
   *
   * @param innerContent - The content inside the brackets.
   * @param outerContent - The content outside the brackets.
   * @return std::string - The formatted string.
   */
  static std::string i_bracket(const std::string &innerContent,
                               const std::string &outerContent) {

    return LogColor::fgGray("[") + innerContent + LogColor::fgGray("]") +
           outerContent;
  }

  /**
   * @brief Wrap content in brackets with specified colors.
   *
   * @param innerContent - The content to wrap.
   * @param outerContent - The content to wrap with.
   * @return std::string - The formatted string.
   */
  static std::string bracket(const std::string &innerContent,
                             const std::string &outerContent) {
    return bracket(innerContent, outerContent, LogColor::fgGray,
                   LogColor::fgCyan, LogColor::fgGray);
  }

  /**
   * @brief Wrap content in brackets with specified colors.
   *
   * @param innerContent - The content to wrap.
   * @param outerContent - The content to wrap with.
   * @param bracketColor - The color of the brackets.
   * @return std::string - The formatted string.
   */
  template <typename LogColorFn>
  static std::string bracket(const std::string &innerContent,
                             const std::string &outerContent,
                             LogColorFn &bracketColor) {
    return bracket(innerContent, outerContent, bracketColor, LogColor::fgCyan,
                   LogColor::fgGray);
  }

  /**
   * @brief Wrap content in brackets with specified colors.
   *
   * @param innerContent - The content to wrap.
   * @param outerContent - The content to wrap with.
   * @param bracketColor - The color of the brackets.
   * @param innerColor   - The color of the inner content.
   * @return std::string - The formatted string.
   */
  template <typename LogColorFn>
  static std::string bracket(const std::string &innerContent,
                             const std::string &outerContent,
                             LogColorFn &bracketColor, LogColorFn &innerColor) {

    return bracket(innerContent, outerContent, bracketColor, innerColor,
                   LogColor::fgGray);
  }

  /**
   * @brief Wrap content in brackets with specified colors.
   *
   * @param innerContent - The content to wrap.
   * @param outerContent - The content to wrap with.
   * @param bracketColor - The color of the brackets.
   * @param innerColor   - The color of the inner content.
   * @param outerColor   - The color of the outer content.
   * @return std::string - The formatted string.
   */
  template <typename LogColorFn>
  static std::string bracket(const std::string &innerContent,
                             const std::string &outerContent,
                             LogColorFn &bracketColor, LogColorFn &innerColor,
                             LogColorFn &outerColor) {

    return bracketColor("[") + innerColor(innerContent) + bracketColor("]") +
           (outerContent.empty() ? "" : " " + outerColor(outerContent));
  }

  /**
   * @brief Check if a string contains another.
   *
   * @param str The string to check.
   * @param pattern The pattern to match.
   * @return true
   * @return false
   */
  static bool contains(const std::string &str, const std::string &pattern);
};

#endif // !STRING_UTILS
