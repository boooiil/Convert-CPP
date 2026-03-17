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
  virtual auto getType() const -> const Encoders_N::Encoders {
    return Encoders_N::INVALID;
  };
  virtual auto getName() const -> const std::string {
    return "BaseVideoCodec";
  };
  virtual auto getDisplayName() const -> const std::string {
    return "Base Video Codec";
  };
  virtual auto getAliases() const -> const std::set<std::string> { return {}; };

  /**
   * @brief The control rate flag used by this codec.
   *
   * @return std::string
   */
  virtual auto controlRateFlag() const -> const std::string = 0;

  // what is a preset ?
  virtual auto supportedPresets() const -> const std::vector<std::string> = 0;
  virtual auto fallbackPreset() const -> const std::string = 0;

  auto getPreset(const std::string &preset) const -> const std::string {
    return getParam(preset, &BaseVideoCodec::supportedPresets,
                    &BaseVideoCodec::fallbackPreset);
  }

  virtual auto supportedLevels() const -> const std::vector<std::string> = 0;
  virtual auto fallbackLevel() const -> const std::string = 0;

  auto getLevel(const std::string &level) const -> const std::string {
    return getParam(level, &BaseVideoCodec::supportedLevels,
                    &BaseVideoCodec::fallbackLevel);
  }

  virtual auto supportedTunes() const -> const std::vector<Tunes_N::Tunes> = 0;
  virtual auto fallbackTune() const -> const Tunes_N::Tunes = 0;

  auto getTune(const Tunes_N::Tunes &tune) const -> const Tunes_N::Tunes {
    return getParam(tune, &BaseVideoCodec::supportedTunes,
                    &BaseVideoCodec::fallbackTune);
  }

  virtual auto getRunningPreset() const -> const std::string & {
    return this->runningPreset;
  }
  virtual void setRunningPreset(const std::string &preset) {
    this->runningPreset = getPreset(preset);
  }

  virtual auto getRunningLevel() const -> const std::string & {
    return this->runningLevel;
  }
  virtual void setRunningLevel(const std::string &level) {
    this->runningLevel = getLevel(level);
  }

  virtual auto getRunningTune() const -> const Tunes_N::Tunes & {
    return this->runningTune;
  }
  virtual void setRunningTune(const Tunes_N::Tunes &tune) {
    this->runningTune = getTune(tune);
  };

private:
  template <typename T, typename C>
  auto getParam(const T &search, const std::vector<T> (C::*provider)() const,
                const T (C::*fallback)() const) const -> const T {
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