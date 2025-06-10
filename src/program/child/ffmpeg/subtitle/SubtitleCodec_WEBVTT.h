#ifndef SUBTITLE_CODEC_WEBVTT_H
#define SUBTITLE_CODEC_WEBVTT_H

#include "../../../settings/enums/StringEnumDataHolder.h"
#include "../../../settings/enums/SubtitleCodec.h"
#include "BaseSubtitleCodec.h"

class SubtitleCodec_WEBVTT : public BaseSubtitleCodec {
 public:
  SubtitleCodec getType() { return SubtitleCodec::WEBVTT; };
};

#endif  // SUBTITLE_CODEC_WEBVTT_H