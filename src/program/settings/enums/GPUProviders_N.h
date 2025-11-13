#ifndef A69162A1_8F7B_47AC_BB5E_971F5EE07DD4
#define A69162A1_8F7B_47AC_BB5E_971F5EE07DD4

#include "src/program/definitions/Definition.h"
#include "src/program/definitions/DefinitionRegistry.h"
#include "src/program/settings/arguments/EnumArgument.h"

class GPUProviders_N {
public:
  enum GPUProviders {
    INVALID = 0,

    NVIDIA,
    AMD,
    INTEL
  };

  static const Definition &definition(GPUProviders provider) {

    switch (provider) {
    case NVIDIA:
      return DefinitionRegistry::get("gpu_nvidia");
    case AMD:
      return DefinitionRegistry::get("gpu_amd");
    case INTEL:
      return DefinitionRegistry::get("gpu_intel");
    default:
      return DefinitionRegistry::get("gpu_invalid");
    }
  }

  static const Definition &definition(EnumArgument<GPUProviders> *arg) {
    return definition(arg->get());
  }

  static const Definition &definition(EnumArgument<GPUProviders> &arg) {
    return definition(arg.get());
  }
};

#endif /* A69162A1_8F7B_47AC_BB5E_971F5EE07DD4 */
