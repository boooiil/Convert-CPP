#ifndef BASE_SUBTITLE_CODEC_H
#define BASE_SUBTITLE_CODEC_H

#include <set>
#include <string>

class BaseSubtitleCodec {
public:
  virtual ~BaseSubtitleCodec() = default;
  virtual auto getName() const -> const std::string {
    return "BaseSubtitleCodec";
  };
  virtual auto getDisplayName() const -> const std::string {
    return "Base Subtitle Codec";
  };
  virtual auto getArg() const -> const std::string { return ""; };
  virtual auto getAliases() const -> const std::set<std::string> { return {}; };
  virtual auto isImage() const -> const bool { return false; };

  void setIndex(const int &idx) { this->index = idx; }
  const int getIndex() const { return this->index; }

  void setMapIndex(const int &idx) { this->mapIndex = idx; }
  const int getMapIndex() const { return this->mapIndex; }

private:
  int index;
  int mapIndex;
};

#endif // BASE_SUBTITLE_CODEC_H