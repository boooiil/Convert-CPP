#ifndef BASE_AUDIO_CODEC_H
#define BASE_AUDIO_CODEC_H

#include <string>
#include <vector>

#include "../../../settings/enums/AudioCodec.h"

class BaseAudioCodec {
 public:
  BaseAudioCodec(int channel, int sampleRate, int bitDepth)
      : runningChannel(channel),
        runningSampleRate(sampleRate),
        runningBitDepth(bitDepth) {};

  virtual auto getType() -> AudioCodec { return AudioCodec::NONE; };
  virtual auto getName() -> std::string { return "BaseAudioCodec"; };
  virtual auto getDisplayName() -> std::string { return "Base Audio Codec"; };

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

  auto setChannel(int channel) -> void {
    this->runningChannel = getChannel(channel);
  };
  auto setSampleRate(int sampleRate) -> void {
    this->runningSampleRate = getSampleRate(sampleRate);
  };
  auto setBitDepth(int bitDepth) -> void {
    this->runningBitDepth = getBitDepth(bitDepth);
  };
  auto setIndex(int index) -> void { this->index = index; };

  auto getRunningChannel() -> const int { return this->runningChannel; };
  auto getRunningSampleRate() -> const int { return this->runningSampleRate; };
  auto getRunningBitDepth() -> const int { return this->runningBitDepth; };
  auto getIndex() -> const int { return this->index; };
  auto getChannelLayout() -> std::string {
    // Return a string representation of the channel layout
    switch (this->runningChannel) {
      case 1:
        return "Mono";
      case 2:
        return "Stereo";
      case 3:
        return "3.0";
      case 4:
        return "Quad";
      case 5:
        return "5.0";
      case 6:
        return "5.1";
      case 7:
        return "6.1";
      case 8:
        return "7.1";
      case 9:
        return "8.1";
      case 10:
        return "9.1";
      default:
        return "unknown";
    }
  }

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

  int index;
  int runningChannel;
  int runningSampleRate;
  int runningBitDepth;
};

#endif  // BASE_AUDIO_CODEC_H