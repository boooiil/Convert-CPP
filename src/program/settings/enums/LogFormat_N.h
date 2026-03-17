#ifndef B5496452_3B0F_4BF1_BF42_4C45B507A310
#define B5496452_3B0F_4BF1_BF42_4C45B507A310

#include "src/program/definitions/Definition.h"
#include "src/program/definitions/DefinitionRegistry.h"
#include "src/program/settings/arguments/EnumArgument.h"

class LogFormat_N {
public:
  enum LogFormat {
    INVALID = 0,

    DEFAULT,
    DEBUG,
    VERBOSE,
    JSON,
    JSON_DEBUG,
    JSON_VERBOSE
  };

  static const Definition &definition(LogFormat logFormat) {

    switch (logFormat) {
    case DEFAULT:
      return DefinitionRegistry::get("log_format_default");
    case DEBUG:
      return DefinitionRegistry::get("log_format_debug");
    case VERBOSE:
      return DefinitionRegistry::get("log_format_verbose");
    case JSON:
      return DefinitionRegistry::get("log_format_json");
    case JSON_DEBUG:
      return DefinitionRegistry::get("log_format_json_debug");
    case JSON_VERBOSE:
      return DefinitionRegistry::get("log_format_json_verbose");
    default:
      return DefinitionRegistry::get("log_format_invalid");
    }
  }

  static const Definition &definition(EnumArgument<LogFormat> *arg) {
    return definition(arg->get());
  }

  static const Definition &definition(EnumArgument<LogFormat> &arg) {
    return definition(arg.get());
  }
};

#endif /* B5496452_3B0F_4BF1_BF42_4C45B507A310 */
