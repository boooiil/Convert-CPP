#ifndef CDCF464E_5BEB_4DA2_BC17_5C57647405AD
#define CDCF464E_5BEB_4DA2_BC17_5C57647405AD

#include "src/program/definitions/Definition.h"
#include "src/program/definitions/DefinitionRegistry.h"
#include "src/program/settings/arguments/EnumArgument.h"

class Platform_N {
public:
  enum Platform {
    INVALID = 0,

    WINDOWS,
    _LINUX,
    LINUX_LIKE,
    MACOS
  };

  static const Definition &definition(Platform platform) {

    switch (platform) {
    case WINDOWS:
      return DefinitionRegistry::get("platform_windows");
    case _LINUX:
      return DefinitionRegistry::get("platform_linux");
    case LINUX_LIKE:
      return DefinitionRegistry::get("platform_linux_like");
    case MACOS:
      return DefinitionRegistry::get("platform_macos");
    default:
      return DefinitionRegistry::get("platform_invalid");
    }
  }

  static const Definition &definition(EnumArgument<Platform> *arg) {
    return definition(arg->get());
  }

  static const Definition &definition(EnumArgument<Platform> &arg) {
    return definition(arg.get());
  }
};
#endif /* CDCF464E_5BEB_4DA2_BC17_5C57647405AD */
