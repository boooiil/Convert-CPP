#ifndef C16C0546_B30F_49F8_8E69_120832731F18
#define C16C0546_B30F_49F8_8E69_120832731F18

#include "src/program/definitions/Definition.h"
#include "src/program/definitions/DefinitionRegistry.h"
#include "src/program/settings/arguments/EnumArgument.h"

// TODO: sync with scripts/data/definitions/definitions_tune.json

class Tunes_N {
public:
  enum Tunes {
    INVALID = 0,

    DEFAULT,
    FAST,
    BALANCED,
    FILM,
    ANIMATION,
    GRAIN,
    STILL_IMAGE,
    PSNR,
    SSIM,
    FAST_DECODE,
    LOW_LATENCY,
    ZERO_LATENCY,
    TEXTURE_COMPRESSION,
    PROXY,
    HIGH_QUALITY,
    LOW_QUALITY,
    ZERO_LATENCY_LOW_LATENCY,
    LOSSLESS
  };

  static const Definition &definition(Tunes tune) {

    switch (tune) {
    case DEFAULT:
      return DefinitionRegistry::get("tune_default");
    case FAST:
      return DefinitionRegistry::get("tune_fast");
    case BALANCED:
      return DefinitionRegistry::get("tune_balanced");
    case FILM:
      return DefinitionRegistry::get("tune_film");
    case ANIMATION:
      return DefinitionRegistry::get("tune_animation");
    case GRAIN:
      return DefinitionRegistry::get("tune_grain");
    case STILL_IMAGE:
      return DefinitionRegistry::get("tune_still_image");
    case PSNR:
      return DefinitionRegistry::get("tune_psnr");
    case SSIM:
      return DefinitionRegistry::get("tune_ssim");
    case FAST_DECODE:
      return DefinitionRegistry::get("tune_fast_decode");
    case LOW_LATENCY:
      return DefinitionRegistry::get("tune_low_latency");
    case ZERO_LATENCY:
      return DefinitionRegistry::get("tune_zero_latency");
    case TEXTURE_COMPRESSION:
      return DefinitionRegistry::get("tune_texture_compression");
    case PROXY:
      return DefinitionRegistry::get("tune_proxy");
    case HIGH_QUALITY:
      return DefinitionRegistry::get("tune_high_quality");
    case LOW_QUALITY:
      return DefinitionRegistry::get("tune_low_quality");
    case ZERO_LATENCY_LOW_LATENCY:
      return DefinitionRegistry::get("tune_zero_latency_low_latency");
    case LOSSLESS:
      return DefinitionRegistry::get("tune_lossless");
    default:
      return DefinitionRegistry::get("tune_invalid");
    }
  }

  static const Definition &definition(EnumArgument<Tunes> *arg) {
    return definition(arg->get());
  }

  static const Definition &definition(EnumArgument<Tunes> &arg) {
    return definition(arg.get());
  }
};

#endif /* C16C0546_B30F_49F8_8E69_120832731F18 */
