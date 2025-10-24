#ifndef BASE_SUBTITLE_CODEC_H
#define BASE_SUBTITLE_CODEC_H

#include <set>
#include <string>

class BaseSubtitleCodec {
public:
  virtual ~BaseSubtitleCodec() = default;
  virtual auto getName() -> std::string { return "BaseSubtitleCodec"; };
  virtual auto getDisplayName() -> std::string {
    return "Base Subtitle Codec";
  };
  virtual auto getAliases() -> std::set<std::string> { return {}; };
};

#endif // BASE_SUBTITLE_CODEC_H