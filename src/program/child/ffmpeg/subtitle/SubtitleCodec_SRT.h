#ifndef SUBTITLE_CODEC_SRT_H
#define SUBTITLE_CODEC_SRT_H

#include "../../../settings/enums/StringEnumDataHolder.h"
#include "../../../settings/enums/SubtitleCodec.h"
#include "BaseSubtitleCodec.h"

class SubtitleCodec_SRT : public BaseSubtitleCodec {
 public:
  auto getType() -> SubtitleCodec override { return SubtitleCodec::SRT; }
  auto getName() -> std::string override { return "srt"; }
};

#endif  // SUBTITLE_CODEC_SRT_H