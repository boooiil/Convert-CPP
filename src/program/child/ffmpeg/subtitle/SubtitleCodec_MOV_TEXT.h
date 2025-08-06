#ifndef SUBTITLE_CODEC_MOV_TEXT_H
#define SUBTITLE_CODEC_MOV_TEXT_H

#include "../../../settings/enums/StringEnumDataHolder.h"
#include "../../../settings/enums/SubtitleCodec.h"
#include "BaseSubtitleCodec.h"

class SubtitleCodec_MOV_TEXT : public BaseSubtitleCodec {
 public:
  auto getType() -> SubtitleCodec override { return SubtitleCodec::MOV_TEXT; }
  auto getName() -> std::string override { return "mov_text"; }
};

#endif  // SUBTITLE_CODEC_MOV_TEXT_H