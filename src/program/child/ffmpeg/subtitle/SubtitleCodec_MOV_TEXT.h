#ifndef SUBTITLE_CODEC_MOV_TEXT_H
#define SUBTITLE_CODEC_MOV_TEXT_H

#include "../../../settings/enums/StringEnumDataHolder.h"
#include "../../../settings/enums/SubtitleCodec.h"
#include "BaseSubtitleCodec.h"

class SubtitleCodec_MOV_TEXT : public BaseSubtitleCodec {
 public:
  SubtitleCodec getType() { return SubtitleCodec::MOV_TEXT; };
};

#endif  // SUBTITLE_CODEC_MOV_TEXT_H