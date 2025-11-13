#ifndef B0B335CF_DF57_4D24_BE54_D9C749E0372C
#define B0B335CF_DF57_4D24_BE54_D9C749E0372C

#include "src/program/definitions/Definition.h"
#include "src/program/definitions/DefinitionRegistry.h"
#include "src/program/settings/arguments/EnumArgument.h"

class HWAccelerators_N {
public:
  enum HWAccelerators {
    INVALID = 0,

    NONE,
    AMD,
    NVIDIA,
    INTEL,
    VULKAN
  };

  static const Definition &definition(HWAccelerators hwaccel) {

    switch (hwaccel) {
    case NONE:
      return DefinitionRegistry::get("hwaccel_none");
    case AMD:
      return DefinitionRegistry::get("hwaccel_amd");
    case NVIDIA:
      return DefinitionRegistry::get("hwaccel_nvidia");
    case INTEL:
      return DefinitionRegistry::get("hwaccel_intel");
    case VULKAN:
      return DefinitionRegistry::get("hwaccel_vulkan");
    default:
      return DefinitionRegistry::get("hwaccel_invalid");
    }
  }

  static const Definition &definition(EnumArgument<HWAccelerators> *arg) {
    return definition(arg->get());
  }

  static const Definition &definition(EnumArgument<HWAccelerators> &arg) {
    return definition(arg.get());
  }
};

#endif /* B0B335CF_DF57_4D24_BE54_D9C749E0372C */
