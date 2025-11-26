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
  virtual auto getArg() -> std::string { return ""; };
  virtual auto getAliases() -> std::set<std::string> { return {}; };
  virtual auto isImage() -> bool { return false; };

  void setIndex(int idx) { this->index = idx; }
  int getIndex() { return this->index; }

  void setMapIndex(int idx) { this->mapIndex = idx; }
  int getMapIndex() { return this->mapIndex; }

private:
  int index;
  int mapIndex;
};

#endif // BASE_SUBTITLE_CODEC_H