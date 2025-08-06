#ifndef SUBTITLE_CODEC_WEBVTT_H
#define SUBTITLE_CODEC_WEBVTT_H

#include "../../../settings/enums/StringEnumDataHolder.h"
#include "../../../settings/enums/SubtitleCodec.h"
#include "BaseSubtitleCodec.h"

class SubtitleCodec_WEBVTT : public BaseSubtitleCodec {
 public:
  auto getType() -> SubtitleCodec override { return SubtitleCodec::WEBVTT; }
  auto getName() -> std::string override { return "webvtt"; }
};

#endif  // SUBTITLE_CODEC_WEBVTT_H