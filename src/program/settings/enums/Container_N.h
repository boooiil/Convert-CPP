#ifndef A0558B5E_8DA4_41D2_B718_DB0938DB8CEC
#define A0558B5E_8DA4_41D2_B718_DB0938DB8CEC

#include "src/program/definitions/Definition.h"
#include "src/program/definitions/DefinitionRegistry.h"
#include "src/program/settings/arguments/EnumArgument.h"

class Container_N {
public:
  enum Container {
    INVALID = 0,

    AVI,
    MKV,
    MOV,
    MP3,
    MP4
  };

  static const Definition &definition(Container container) {

    switch (container) {
    case AVI:
      return DefinitionRegistry::get("container_avi");
    case MKV:
      return DefinitionRegistry::get("container_mkv");
    case MOV:
      return DefinitionRegistry::get("container_mov");
    case MP3:
      return DefinitionRegistry::get("container_mp3");
    case MP4:
      return DefinitionRegistry::get("container_mp4");
    default:
      return DefinitionRegistry::get("container_invalid");
    }
  }

  static const Definition &definition(EnumArgument<Container> *arg) {
    return definition(arg->get());
  }

  static const Definition &definition(EnumArgument<Container> &arg) {
    return definition(arg.get());
  }
};

#endif /* A0558B5E_8DA4_41D2_B718_DB0938DB8CEC */
