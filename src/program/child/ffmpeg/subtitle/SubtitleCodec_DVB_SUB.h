#ifndef SUBTITLE_CODEC_DVB_SUB_H
#define SUBTITLE_CODEC_DVB_SUB_H

#include "../../../settings/enums/StringEnumDataHolder.h"
#include "../../../settings/enums/SubtitleCodec.h"
#include "BaseSubtitleCodec.h"

class SubtitleCodec_DVB_SUB : public BaseSubtitleCodec {
 public:
  SubtitleCodec getType() { return SubtitleCodec::DVB_SUB; };
};

#endif  // SUBTITLE_CODEC_DVB_SUB_H