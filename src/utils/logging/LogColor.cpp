#include "LogColor.h"

/** ********************** *
 *      TERMINAL CODES     *
 * *********************** */

std::string LogColor::reset(std::string str) {
  return "\033[0m" + str + "\x1b[0m";
}
std::string LogColor::bright(std::string str) {
  return "\033[1m" + str + "\x1b[0m";
}
std::string LogColor::dim(std::string str) {
  return "\033[2m" + str + "\x1b[0m";
}
std::string LogColor::underscore(std::string str) {
  return "\033[4m" + str + "\x1b[0m";
}
std::string LogColor::blink(std::string str) {
  return "\033[5m" + str + "\x1b[0m";
}
std::string LogColor::reverse(std::string str) {
  return "\033[7m" + str + "\x1b[0m";
}
std::string LogColor::hidden(std::string str) {
  return "\033[8m" + str + "\x1b[0m";
}
std::string LogColor::none(std::string str) {
  return "\033[0m" + str + "\x1b[0m";
}

/** ********************** *
 *        TEXT CODES       *
 * *********************** */

std::string LogColor::fgBlack(std::string str) {
  return "\x1b[38;5;0m" + str + "\x1b[0m";
}
std::string LogColor::fgRed(std::string str) {
  return "\x1b[38;5;196m" + str + "\x1b[0m";
}
std::string LogColor::fgGreen(std::string str) {
  return "\x1b[38;5;40m" + str + "\x1b[0m";
}
std::string LogColor::fgGray(std::string str) {
  return "\x1b[38;5;8m" + str + "\x1b[0m";
}
std::string LogColor::fgYellow(std::string str) {
  return "\x1b[38;5;226m" + str + "\x1b[0m";
}
std::string LogColor::fgBlue(std::string str) {
  return "\x1b[38;5;33m" + str + "\x1b[0m";
}
std::string LogColor::fgMagenta(std::string str) {
  return "\x1b[38;5;99m" + str + "\x1b[0m";
}
std::string LogColor::fgCyan(std::string str) {
  return "\x1b[38;5;81m" + str + "\x1b[0m";
}
std::string LogColor::fgOrange(std::string str) {
  return "\x1b[38;5;214m" + str + "\x1b[0m";
}
std::string LogColor::fgWhite(std::string str) {
  return "\x1b[38;5;15m" + str + "\x1b[0m";
}

/** ********************** *
 *     BACKGROUND CODES    *
 * *********************** */

std::string LogColor::bgBlack(std::string str) {
  return "\x1b[38;5;0m" + str + "\x1b[0m";
}
std::string LogColor::bgRed(std::string str) {
  return "\x1b[38;5;196m" + str + "\x1b[0m";
}
std::string LogColor::bgGreen(std::string str) {
  return "\x1b[38;5;40m" + str + "\x1b[0m";
}
std::string LogColor::bgGray(std::string str) {
  return "\x1b[38;5;8m" + str + "\x1b[0m";
}
std::string LogColor::bgYellow(std::string str) {
  return "\x1b[38;5;226m" + str + "\x1b[0m";
}
std::string LogColor::bgBlue(std::string str) {
  return "\x1b[38;5;33m" + str + "\x1b[0m";
}
std::string LogColor::bgMagenta(std::string str) {
  return "\x1b[38;5;99m" + str + "\x1b[0m";
}
std::string LogColor::bgCyan(std::string str) {
  return "\x1b[38;5;81m" + str + "\x1b[0m";
}
std::string LogColor::bgOrange(std::string str) {
  return "\x1b[38;5;214m" + str + "\x1b[0m";
}
std::string LogColor::bgWhite(std::string str) {
  return "\x1b[38;5;15m" + str + "\x1b[0m";
}
