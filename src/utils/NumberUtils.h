/*********************************************************************
 * @file   NumberUtils.h
 * @brief  Number utilities class header.
 *
 * @author boooiil
 * @date   January 2024
 *********************************************************************/

#ifndef NUMBER_UTILS_H
#define NUMBER_UTILS_H

#include <string>

/**
 * @brief Number utilities class.
 */
class NumberUtils {
 public:
  /**
   * @brief Format number to string.
   *
   * @param[in] number - number to format.
   * @param[in] digits - number of digits after decimal point.
   * @return Formatted number.
   */
  static std::string formatNumber(float number, int digits);

  /**
   * @brief Format number to string.
   *
   * @param[in] number - number to format.
   * @param[in] digits - number of digits after decimal point.
   * @return Formatted number.
   */
  static std::string formatNumber(double number, int digits);
};

#endif  // !NUMBER_UTILS_H
