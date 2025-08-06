#ifndef BASE_SUBTITLE_CODEC_H
#define BASE_SUBTITLE_CODEC_H

#include <vector>

#include "../../../settings/enums/StringEnumDataHolder.h"
#include "../../../settings/enums/SubtitleCodec.h"

class BaseSubtitleCodec {
 public:
  virtual auto getType() -> SubtitleCodec { return SubtitleCodec::NONE; };
  virtual auto getName() -> std::string { return "BaseSubtitleCodec"; };
};

#endif  // BASE_SUBTITLE_CODEC_H