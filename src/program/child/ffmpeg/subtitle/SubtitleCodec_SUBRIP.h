#ifndef SUBTITLE_CODEC_SUBRIP_H
#define SUBTITLE_CODEC_SUBRIP_H

#include "../../../settings/enums/StringEnumDataHolder.h"
#include "../../../settings/enums/SubtitleCodec.h"
#include "BaseSubtitleCodec.h"

class SubtitleCodec_SUBRIP : public BaseSubtitleCodec {
 public:
  auto getType() -> SubtitleCodec override { return SubtitleCodec::SUBRIP; }
  auto getName() -> std::string override { return "subrip"; }
};

#endif  // SUBTITLE_CODEC_SUBRIP_H