#ifndef SUBTITLE_CODEC_SRT_H
#define SUBTITLE_CODEC_SRT_H

#include "../../../settings/enums/StringEnumDataHolder.h"
#include "../../../settings/enums/SubtitleCodec.h"
#include "BaseSubtitleCodec.h"

class SubtitleCodec_SRT : public BaseSubtitleCodec {
 public:
  SubtitleCodec getType() { return SubtitleCodec::SRT; };
};

#endif  // SUBTITLE_CODEC_SRT_H