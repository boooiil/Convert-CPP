#ifndef E7DE2481_0659_42FA_A0BC_F2852AB87AF0
#define E7DE2481_0659_42FA_A0BC_F2852AB87AF0

#include "src/program/definitions/Definition.h"
#include "src/program/definitions/DefinitionRegistry.h"

class SubtitleCodecs_N {
public:
  enum SubtitleCodec {
    INVALID = 0,

    NONE,
    ASS,
    SRT,
    SSA,
    SUBRIP,
    DVB_SUB,
    WEBVTT,
    MOV_TEXT
  };

  static const Definition &definition(SubtitleCodec subtitleCodec) {

    switch (subtitleCodec) {
    case NONE:
      return DefinitionRegistry::get("subtitle_none");
    case ASS:
      return DefinitionRegistry::get("subtitle_ass");
    case SRT:
      return DefinitionRegistry::get("subtitle_srt");
    case SSA:
      return DefinitionRegistry::get("subtitle_ssa");
    case SUBRIP:
      return DefinitionRegistry::get("subtitle_subrip");
    case DVB_SUB:
      return DefinitionRegistry::get("subtitle_dvb_sub");
    case WEBVTT:
      return DefinitionRegistry::get("subtitle_webvtt");
    case MOV_TEXT:
      return DefinitionRegistry::get("subtitle_mov_text");
    default:
      return DefinitionRegistry::get("subtitle_invalid");
    }
  }
};

#endif /* E7DE2481_0659_42FA_A0BC_F2852AB87AF0 */
