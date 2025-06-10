#ifndef BASE_VIDEO_CODEC_H
#define BASE_VIDEO_CODEC_H

#include <set>
#include <vector>

#include "../../../../utils/logging/Logger.h"
#include "../../../settings/enums/Encoders.h"
#include "../../../settings/enums/MediaFormats.h"
#include "../../../settings/enums/ObjectEnumDataHolder.h"
#include "../../../settings/enums/StringEnumDataHolder.h"
#include "../../../settings/enums/Tunes.h"

class BaseVideoCodec {
 public:
  virtual ~BaseVideoCodec() {
    LOG_DEBUG("Deconstructing...");
    if (runningTune != nullptr) {
      LOG_DEBUG("Deleting Running Tune");
      delete runningTune;
    }
  }

  /**
   * @brief Get the type of video codec used by this encoder
   *
   * @return Encoders
   */
  virtual auto getType() -> Encoders { return Encoders::INVALID; };

  /**
   * @brief The control rate flag used by this codec.
   *
   * @return std::string
   */
  virtual auto controlRateFlag() -> std::string = 0;

  // what is a preset ?
  virtual auto supportedPresets() -> std::vector<std::string> = 0;
  virtual auto fallbackPreset() -> std::string = 0;

  auto getPreset(std::string& preset) -> const std::string {
    return getParam(preset, &BaseVideoCodec::supportedPresets,
                    &BaseVideoCodec::fallbackPreset);
  }

  virtual auto supportedLevels() -> std::vector<std::string> = 0;
  virtual auto fallbackLevel() -> std::string = 0;

  auto getLevel(std::string& level) -> const std::string {
    return getParam(level, &BaseVideoCodec::supportedLevels,
                    &BaseVideoCodec::fallbackLevel);
  }

  virtual auto supportedTunes() -> std::vector<Tunes> = 0;
  virtual auto fallbackTune() -> Tunes = 0;

  auto getTune(Tunes& tune) -> const Tunes {
    return getParam(tune, &BaseVideoCodec::supportedTunes,
                    &BaseVideoCodec::fallbackTune);
  }

  virtual auto getRunningPreset() -> const std::string& {
    return this->runningPreset;
  }
  virtual auto setRunningPreset(std::string& preset) -> void {
    this->runningPreset = preset;
  }

  virtual auto getRunningLevel() -> const std::string& {
    return this->runningLevel;
  }
  virtual auto setRunningLevel(std::string& level) -> const std::string& {
    this->runningLevel = level;
  }

  virtual auto getRunningTune() -> const Tunes& { return *this->runningTune; }
  virtual auto setRunningTune(Tunes* tune) -> void { this->runningTune = tune; }

 private:
  template <typename T, typename C>
  auto getParam(T& search, std::vector<T> (C::*provider)(), T (C::*fallback)())
      -> const T {
    auto params = (this->*provider)();

    for (auto& p : params) {
      if (p == search) {
        return p;
      }
    }
    return (this->*fallback)();
  }

  std::string runningPreset;
  std::string runningLevel;
  Tunes* runningTune;
};

#endif  // BASE_VIDEO_CODEC_H