#ifndef F35EFF37_852F_45DC_9D7C_F5C9D7B165CB
#define F35EFF37_852F_45DC_9D7C_F5C9D7B165CB

#include "src/program/definitions/Definition.h"
#include "src/program/definitions/DefinitionRegistry.h"
#include "src/program/settings/arguments/EnumArgument.h"

class AudioCodec_N {
public:
  enum AudioCodec {
    INVALID = 0,

    NONE,
    AAC,
    AC3,
    EAC3,
    MP3,
    FLAC,
    TRUEHD,
    VORBIS,
    OPUS
  };

  static const Definition &definition(AudioCodec audioCodec) {

    switch (audioCodec) {
    case NONE:
      return DefinitionRegistry::get("audio_none");
    case AAC:
      return DefinitionRegistry::get("audio_aac");
    case AC3:
      return DefinitionRegistry::get("audio_ac3");
    case EAC3:
      return DefinitionRegistry::get("audio_eac3");
    case MP3:
      return DefinitionRegistry::get("audio_mp3");
    case FLAC:
      return DefinitionRegistry::get("audio_flac");
    case TRUEHD:
      return DefinitionRegistry::get("audio_truehd");
    case VORBIS:
      return DefinitionRegistry::get("audio_vorbis");
    case OPUS:
      return DefinitionRegistry::get("audio_opus");
    default:
      return DefinitionRegistry::get("audio_invalid");
    }
  }

  static const Definition &definition(EnumArgument<AudioCodec> *arg) {
    return definition(arg->get());
  }

  static const Definition &definition(EnumArgument<AudioCodec> &arg) {
    return definition(arg.get());
  }
};

#endif /* F35EFF37_852F_45DC_9D7C_F5C9D7B165CB */
