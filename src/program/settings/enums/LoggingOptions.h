#ifndef LOGGING_OPTIONS_H
#define LOGGING_OPTIONS_H

#include <string>
#include <unordered_map>
#include <vector>

#include "StringEnumDataHolder.h"

enum class LoggingOptions {
  INVALID = 0,

  DEFAULT,
  DEBUG,
  VERBOSE,
  JSON,
  JSON_DEBUG,
  JSON_VERBOSE
  //public:
  // static const StringEnumDataHolder<LoggingOptions> INVALID;
  // static const StringEnumDataHolder<LoggingOptions> DEFAULT;
  // static const StringEnumDataHolder<LoggingOptions> DEBUG;
  // static const StringEnumDataHolder<LoggingOptions> VERBOSE;
  // static const StringEnumDataHolder<LoggingOptions> JSON;
  // static const StringEnumDataHolder<LoggingOptions> JSON_DEBUG;
  // static const StringEnumDataHolder<LoggingOptions> JSON_VERBOSE;

  // static const std::vector<const StringEnumDataHolder<LoggingOptions>*> _all() {
  //   return {&INVALID, &DEFAULT,    &DEBUG,       &VERBOSE,
  //           &JSON,    &JSON_DEBUG, &JSON_VERBOSE};
  // }

  // static const StringEnumDataHolder<LoggingOptions> getKey(std::string value) {
  //   for (auto& item : _all()) {
  //     if (item->getName() == value) {
  //       return *item;
  //     }
  //   }
  //   return INVALID;
  // }

  // /**
  //  * @brief Check if the provided option is JSON.
  //  *
  //  * @param[in] providedOption - Desired logging option.
  //  * @return True if JSON, false otherwise.
  //  */
  // static bool isJSON(StringEnumDataHolder<LoggingOptions> providedOption);

  // /**
  //  * @brief Check if the provided option is Debug.
  //  *
  //  * @param[in] providedOption - Desired logging option.
  //  * @return True if Debug, false otherwise.
  //  */
  // static bool isDebug(StringEnumDataHolder<LoggingOptions> providedOption);

  // /**
  //  * @brief Check if the provided option is Verbose.
  //  *   *
  //  * @param[in] providedOption - Desired logging option.
  //  *   * @return True if Verbose, false otherwise.
  //  */
  // static bool isVerbose(StringEnumDataHolder<LoggingOptions> providedOption);
};

#endif  // LOGGING_OPTIONS_H
