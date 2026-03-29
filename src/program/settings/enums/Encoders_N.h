#ifndef FC95D64C_76B5_4CF7_A795_C1A1CB4D27BE
#define FC95D64C_76B5_4CF7_A795_C1A1CB4D27BE

#include "src/program/definitions/Definition.h"
#include "src/program/definitions/DefinitionRegistry.h"
#include "src/program/settings/arguments/EnumArgument.h"
#include "src/utils/logging/LogColor.h"

class Encoders_N {
public:
  enum Encoders {
    INVALID = 0,

    H264,
    H264_AMF,
    H264_NVENC,
    H264_QSV,

    HEVC,
    HEVC_AMF,
    HEVC_NVENC,
    HEVC_QSV,

    AV1,
    AV1_AMF,
    AV1_NVENC,
    AV1_QSV
  };

  static const Definition &definition(Encoders encoder) {

    switch (encoder) {
    case H264:
      return DefinitionRegistry::get("encoder_h264");
    case H264_AMF:
      return DefinitionRegistry::get("encoder_h264_amf");
    case H264_NVENC:
      return DefinitionRegistry::get("encoder_h264_nvenc");
    case H264_QSV:
      return DefinitionRegistry::get("encoder_h264_qsv");
    case HEVC:
      return DefinitionRegistry::get("encoder_hevc");
    case HEVC_AMF:
      return DefinitionRegistry::get("encoder_hevc_amf");
    case HEVC_NVENC:
      return DefinitionRegistry::get("encoder_hevc_nvenc");
    case HEVC_QSV:
      return DefinitionRegistry::get("encoder_hevc_qsv");
    case AV1:
      return DefinitionRegistry::get("encoder_av1");
    case AV1_AMF:
      return DefinitionRegistry::get("encoder_av1_amf");
    case AV1_NVENC:
      return DefinitionRegistry::get("encoder_av1_nvenc");
    case AV1_QSV:
      return DefinitionRegistry::get("encoder_av1_qsv");
    default:
      return DefinitionRegistry::get("encoder_invalid");
    }
  }

  static const std::string getLetter(Encoders encoder) {
    return isSoftwareEncoder(encoder) ? LogColor::fgWhite("S")
                                      : LogColor::fgGreen("H");
  }

  static const bool isSoftwareEncoder(Encoders encoder) {
    switch (encoder) {
    case H264:
    case HEVC:
    case AV1:
      return true;
    default:
      return false;
    }
  }

  static const bool isHardwareEncoder(Encoders encoder) {
    return !isSoftwareEncoder(encoder);
  }

  static const Definition &definition(EnumArgument<Encoders> *arg) {
    return definition(arg->get());
  }

  static const Definition &definition(EnumArgument<Encoders> &arg) {
    return definition(arg.get());
  }
};

#endif /* FC95D64C_76B5_4CF7_A795_C1A1CB4D27BE */
