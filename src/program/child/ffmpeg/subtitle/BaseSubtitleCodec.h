#ifndef BASE_SUBTITLE_CODEC_H
#define BASE_SUBTITLE_CODEC_H

#include "../../../settings/enums/SubtitleCodec.h"
#include <set>
#include <string>

class BaseSubtitleCodec {
public:
  virtual ~BaseSubtitleCodec() = default;
  virtual auto getType() -> SubtitleCodec { return SubtitleCodec::NONE; };
  virtual auto getName() -> std::string { return "BaseSubtitleCodec"; };
  virtual auto getAliases() -> std::set<std::string> { return {}; };
};

#endif // BASE_SUBTITLE_CODEC_H