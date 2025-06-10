#ifndef BASE_AUDIO_CODEC_H
#define BASE_AUDIO_CODEC_H

#include <vector>

#include "../../../settings/enums/AudioCodec.h"

class BaseAudioCodec {
 public:
  virtual auto getType() -> AudioCodec { return AudioCodec::NONE; };

  virtual auto supportedChannels() -> std::vector<int> = 0;
  virtual auto fallbackChannel() -> int = 0;

  virtual auto getChannel(int channel) -> const int {
    return getParam(channel, &BaseAudioCodec::supportedChannels,
                    &BaseAudioCodec::fallbackChannel);
  }

  virtual auto supportedSampleRates() -> std::vector<int> = 0;
  virtual auto fallbackSampleRate() -> int = 0;

  virtual auto getSampleRate(int sampleRate) -> const int {
    return getParam(sampleRate, &BaseAudioCodec::supportedSampleRates,
                    &BaseAudioCodec::fallbackSampleRate);
  }

  virtual auto supportedBitDepths() -> std::vector<int> = 0;
  virtual auto fallbackBitDepth() -> int = 0;

  virtual auto getBitDepth(int bitDepth) -> const int {
    return getParam(bitDepth, &BaseAudioCodec::supportedBitDepths,
                    &BaseAudioCodec::fallbackBitDepth);
  }

  virtual auto getRunningChannel() -> const int {
    return this->runningChannel;
  };
  virtual auto setRunningChannel(int channel) -> void {
    this->runningChannel = channel;
  };

  virtual auto getRunningSampleRate() -> const int {
    return this->runningSampleRate;
  };
  virtual auto setRunningSampleRate(int channel) -> void {
    this->runningSampleRate = channel;
  };

  virtual auto getRunningBitDepth() -> const int {
    return this->runningBitDepth;
  };
  virtual auto setRunningBitDepth(int channel) -> void {
    this->runningBitDepth = channel;
  };

 private:
  template <typename T, typename C>
  auto getParam(T search, std::vector<T> (C::*provider)(), T (C::*fallback)())
      -> const T {
    auto params = (this->*provider)();

    for (auto& p : params) {
      if (p == search) {
        return p;
      }
    }
    return (this->*fallback)();
  }

  int runningChannel = 2;
  int runningSampleRate = 41000;
  int runningBitDepth = 16;
};

#endif  // BASE_AUDIO_CODEC_H