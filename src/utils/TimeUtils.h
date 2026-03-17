/*********************************************************************
 * @file   TimeUtils.h
 * @brief  TimeUtils class definition.
 *
 * @author boooiil
 * @date   January 2024
 *********************************************************************/

#ifndef TIME_UTILS_H
#define TIME_UTILS_H

#include <string>

/**
 * @brief Time helper class.
 */
class TimeUtils {
 public:
  static long getTime();
  static long getEpoch();

  /**
   * @brief Format of HH:MM:SS over an elapsed time in milliseconds.
   *
   * @param[in] ms - elapsed time in milliseconds
   * @return formatted time
   */
  static std::string durationFormat(long ms);

  /**
   * @brief Format of HH:MM:SS - MM/DD/YYYY over an epoch.
   *
   * @param[in] epoch - epoch time
   * @return formatted date
   */
  static std::string dateFormat(long epoch);

  /**
   * @brief Format of HH:MM:SS-(AM/PM) over an epoch.
   *
   * @param[in] epoch - epoch time
   * @return formatted time
   */
  static std::string timeFormat(long epoch);
};

#endif  // !TIME_UTILS_H
