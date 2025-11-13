#ifndef BASE_VIDEO_CODEC_H
#define BASE_VIDEO_CODEC_H

#include <set>
#include <vector>

#include "../../../settings/enums/Encoders_N.h"
#include "../../../settings/enums/Tunes_N.h"

class BaseVideoCodec {
public:
  BaseVideoCodec(std::string runningPreset, std::string runningLevel,
                 Tunes_N::Tunes runningTune)
      : runningPreset(runningPreset), runningLevel(runningLevel),
        runningTune(runningTune) {};
  virtual ~BaseVideoCodec() {}

  /**
   * @brief Get the type of video codec used by this encoder
   *
   * @return Encoders
   */
  virtual auto getType() -> Encoders_N::Encoders {
    return Encoders_N::INVALID;
  };
  virtual auto getName() -> std::string { return "BaseVideoCodec"; };
  virtual auto getDisplayName() -> std::string { return "Base Video Codec"; };
  virtual auto getAliases() -> std::set<std::string> { return {}; };

  /**
   * @brief The control rate flag used by this codec.
   *
   * @return std::string
   */
  virtual auto controlRateFlag() -> std::string = 0;

  // what is a preset ?
  virtual auto supportedPresets() -> std::vector<std::string> = 0;
  virtual auto fallbackPreset() -> std::string = 0;

  auto getPreset(std::string &preset) -> const std::string {
    return getParam(preset, &BaseVideoCodec::supportedPresets,
                    &BaseVideoCodec::fallbackPreset);
  }

  virtual auto supportedLevels() -> std::vector<std::string> = 0;
  virtual auto fallbackLevel() -> std::string = 0;

  auto getLevel(std::string &level) -> const std::string {
    return getParam(level, &BaseVideoCodec::supportedLevels,
                    &BaseVideoCodec::fallbackLevel);
  }

  virtual auto supportedTunes() -> std::vector<Tunes_N::Tunes> = 0;
  virtual auto fallbackTune() -> Tunes_N::Tunes = 0;

  auto getTune(Tunes_N::Tunes &tune) -> const Tunes_N::Tunes {
    return getParam(tune, &BaseVideoCodec::supportedTunes,
                    &BaseVideoCodec::fallbackTune);
  }

  virtual auto getRunningPreset() -> const std::string & {
    return this->runningPreset;
  }
  virtual void setRunningPreset(std::string &preset) {
    this->runningPreset = getPreset(preset);
  }

  virtual auto getRunningLevel() -> const std::string & {
    return this->runningLevel;
  }
  virtual void setRunningLevel(std::string &level) {
    this->runningLevel = getLevel(level);
  }

  virtual auto getRunningTune() -> const Tunes_N::Tunes & {
    return this->runningTune;
  }
  virtual void setRunningTune(Tunes_N::Tunes tune) {
    this->runningTune = getTune(tune);
  };

private:
  template <typename T, typename C>
  auto getParam(T &search, std::vector<T> (C::*provider)(), T (C::*fallback)())
      -> const T {
    auto params = (this->*provider)();

    for (auto &p : params) {
      if (p == search) {
        return p;
      }
    }
    return (this->*fallback)();
  }

  std::string runningPreset;
  std::string runningLevel;
  Tunes_N::Tunes runningTune;
};

#endif // BASE_VIDEO_CODEC_H