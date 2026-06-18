#ifndef BASE_AUDIO_CODEC_H
#define BASE_AUDIO_CODEC_H

#include <set>
#include <string>
#include <vector>

class BaseAudioCodec {
public:
  virtual ~BaseAudioCodec() = default;
  BaseAudioCodec(int channel, int sampleRate, std::string bitDepth)
      : runningChannel(channel), runningSampleRate(sampleRate),
        runningBitDepth(bitDepth), runningBitrate(-1) {};

  virtual auto getName() const -> const std::string {
    return "BaseAudioCodec";
  };
  virtual auto getDisplayName() const -> const std::string {
    return "Base Audio Codec";
  };
  virtual auto getAliases() const -> const std::set<std::string> { return {}; };
  virtual auto supportedChannels() const -> const std::vector<int> = 0;
  virtual auto fallbackChannel() const -> const int = 0;

  virtual auto getChannel(const int &channel) const -> const int {
    return getParam(channel, &BaseAudioCodec::supportedChannels,
                    &BaseAudioCodec::fallbackChannel);
  }

  virtual auto supportedSampleRates() const -> const std::vector<int> = 0;
  virtual auto fallbackSampleRate() const -> const int = 0;

  virtual auto getSampleRate(const int &sampleRate) const -> const int {
    return getParam(sampleRate, &BaseAudioCodec::supportedSampleRates,
                    &BaseAudioCodec::fallbackSampleRate);
  }

  virtual auto supportedBitDepths() const -> const std::vector<std::string> = 0;
  virtual auto fallbackBitDepth() const -> const std::string = 0;

  virtual auto getBitDepth(const std::string &bitDepth) const
      -> const std::string {
    return getParam(bitDepth, &BaseAudioCodec::supportedBitDepths,
                    &BaseAudioCodec::fallbackBitDepth);
  }

  auto setChannel(const int &channel) -> void {
    this->runningChannel = getChannel(channel);
  };
  auto setSampleRate(const int &sampleRate) -> void {
    this->runningSampleRate = getSampleRate(sampleRate);
  };
  auto setBitDepth(const std::string &bitDepth) -> void {
    this->runningBitDepth = getBitDepth(bitDepth);
  };
  auto setBitrate(const int &bitrate) -> void {
    this->runningBitrate = bitrate;
  };
  auto setIndex(const int &index) -> void { this->index = index; };
  auto setMapIndex(const int &mapIndex) -> void { this->mapIndex = mapIndex; };

  auto getRunningChannel() const -> const int { return this->runningChannel; };
  auto getRunningSampleRate() const -> const int {
    return this->runningSampleRate;
  };
  auto getRunningBitDepth() const -> const std::string {
    return this->runningBitDepth;
  };
  auto getRunningBitrate() const -> const int { return this->runningBitrate; };
  auto getIndex() const -> const int { return this->index; };
  auto getMapIndex() const -> const int { return this->mapIndex; };
  auto getChannelLayout() const -> const std::string {
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

  int index;
  int mapIndex;
  int runningChannel;
  int runningSampleRate;
  std::string runningBitDepth;
  int runningBitrate;
};

#endif // BASE_AUDIO_CODEC_H