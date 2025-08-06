#ifndef SUBTITLE_CODEC_ASS_H
#define SUBTITLE_CODEC_ASS_H

#include "../../../settings/enums/StringEnumDataHolder.h"
#include "../../../settings/enums/SubtitleCodec.h"
#include "BaseSubtitleCodec.h"

class SubtitleCodec_ASS : public BaseSubtitleCodec {
 public:
  auto getType() -> SubtitleCodec override { return SubtitleCodec::ASS; }
  auto getName() -> std::string override { return "ass"; }
};

#endif  // SUBTITLE_CODEC_ASS_H