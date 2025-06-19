/*********************************************************************
 * @file   Platform.h
 * @brief  Platform class header.
 *
 * @author boooiil
 * @date   January 2024
 *********************************************************************/

#ifndef PLATFORM_H
#define PLATFORM_H

#include <string>
#include <vector>

#include "StringEnumDataHolder.h"

enum class Platform {
  INVALID = 0,

  WINDOWS,
  _LINUX,
  LINUX_LIKE,
  MACOS

  // public:
  //  static const StringEnumDataHolder<Platform> UNKNOWN;

  // static const StringEnumDataHolder<Platform> WINDOWS;
  // static const StringEnumDataHolder<Platform> _LINUX;
  // static const StringEnumDataHolder<Platform> LINUX_LIKE;
  // static const StringEnumDataHolder<Platform> MACOS;

  // static const std::vector<const StringEnumDataHolder<Platform>*> _all() {
  //   return {&WINDOWS, &_LINUX, &LINUX_LIKE, &MACOS};
  // }

  // /**
  //  * @brief Get the string value of the operating system.
  //  *
  //  * @param[in] os - The operating system.
  //  * @return The string value of the operating system.
  //  */
  // static const StringEnumDataHolder<Platform> getKey(std::string value) {
  //   for (auto& item : _all()) {
  //     if (item->getName() == value) {
  //       return *item;
  //     }
  //   }
  //   return UNKNOWN;
  // }
};

#endif  // !PLATFORM_H
