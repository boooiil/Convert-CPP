/*********************************************************************
 * @file   LogColor.h
 * @brief  Log Color class header.
 *
 * @author boooiil
 * @date   January 2024
 *********************************************************************/

#if !defined(LOG_COLOR)
#define LOG_COLOR

#include <string>

/**
 * @brief Static class to colorize the terminal output.
 */
class LogColor {
 public:
  /** ********************** *
   *      TERMINAL CODES     *
   * *********************** */

  static std::string reset(std::string);
  static std::string bright(std::string);
  static std::string dim(std::string);
  static std::string underscore(std::string);
  static std::string blink(std::string);
  static std::string reverse(std::string);
  static std::string hidden(std::string);
  static std::string none(std::string);

  /** ********************** *
   *        TEXT CODES       *
   * *********************** */

  static std::string fgBlack(std::string);
  static std::string fgRed(std::string);
  static std::string fgGreen(std::string);
  static std::string fgGray(std::string);
  static std::string fgYellow(std::string);
  static std::string fgBlue(std::string);
  static std::string fgMagenta(std::string);
  static std::string fgCyan(std::string);
  static std::string fgOrange(std::string);
  static std::string fgWhite(std::string);

  /** ********************** *
   *     BACKGROUND CODES    *
   * *********************** */

  static std::string bgBlack(std::string);
  static std::string bgRed(std::string);
  static std::string bgGreen(std::string);
  static std::string bgGray(std::string);
  static std::string bgYellow(std::string);
  static std::string bgBlue(std::string);
  static std::string bgMagenta(std::string);
  static std::string bgCyan(std::string);
  static std::string bgOrange(std::string);
  static std::string bgWhite(std::string);

 private:
};

#endif  // LOG_COLOR
