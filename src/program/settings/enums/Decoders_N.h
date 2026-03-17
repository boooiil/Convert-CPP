#ifndef B1469E0D_0F2F_4402_8E6C_C70BD1010A95
#define B1469E0D_0F2F_4402_8E6C_C70BD1010A95

#include "src/program/definitions/Definition.h"
#include "src/program/definitions/DefinitionRegistry.h"
#include "src/program/settings/arguments/EnumArgument.h"

class Decoders_N {
public:
  enum Decoders {
    INVALID = 0,

    NONE,
    AV1_CUVID,
    AV1_QSV,
    H264_CUVID,
    H264_QSV,
    HEVC_CUVID,
    HEVC_QSV
  };

  static const Definition &definition(Decoders decoder) {

    switch (decoder) {
    case AV1_CUVID:
      return DefinitionRegistry::get("decoder_av1_cuvid");
    case AV1_QSV:
      return DefinitionRegistry::get("decoder_av1_qsv");
    case H264_CUVID:
      return DefinitionRegistry::get("decoder_h264_cuvid");
    case H264_QSV:
      return DefinitionRegistry::get("decoder_h264_qsv");
    case HEVC_CUVID:
      return DefinitionRegistry::get("decoder_hevc_cuvid");
    case HEVC_QSV:
      return DefinitionRegistry::get("decoder_hevc_qsv");
    default:
      return DefinitionRegistry::get("decoder_invalid");
    }
  }

  static const Definition &definition(EnumArgument<Decoders> *arg) {
    return definition(arg->get());
  }

  static const Definition &definition(EnumArgument<Decoders> &arg) {
    return definition(arg.get());
  }
};

#endif /* B1469E0D_0F2F_4402_8E6C_C70BD1010A95 */
