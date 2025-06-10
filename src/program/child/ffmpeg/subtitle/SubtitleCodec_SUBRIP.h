#ifndef SUBTITLE_CODEC_SUBRIP_H
#define SUBTITLE_CODEC_SUBRIP_H

#include "../../../settings/enums/StringEnumDataHolder.h"
#include "../../../settings/enums/SubtitleCodec.h"
#include "BaseSubtitleCodec.h"

class SubtitleCodec_SUBRIP : public BaseSubtitleCodec {
 public:
  SubtitleCodec getType() { return SubtitleCodec::SUBRIP; };
};

#endif  // SUBTITLE_CODEC_SUBRIP_H