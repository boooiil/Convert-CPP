#ifndef AUDIO_CODEC_MP3_H
#define AUDIO_CODEC_MP3_H

#include "../../../settings/enums/AudioCodec.h"
#include "../../../settings/enums/StringEnumDataHolder.h"
#include "BaseAudioCodec.h"

class AudioCodec_MP3 : public BaseAudioCodec {
 public:
  AudioCodec_MP3(int channel = 2, int sampleRate = 48000, int bitDepth = 16)
      : BaseAudioCodec(channel, sampleRate, bitDepth) {};

  auto getType() -> AudioCodec override { return AudioCodec::MP3; };
  auto getName() -> std::string override { return "mp3"; };
  auto getDisplayName() -> std::string override { return "MP3"; };

  auto supportedChannels() -> std::vector<int> override {
    return {
        1,  // mono
        2   // l, r
    };
  };
  auto fallbackChannel() -> int override { return 2; };

  auto supportedSampleRates() -> std::vector<int> override {
    return {8000, 11025, 12000, 16000, 22050, 24000, 32000, 44100, 48000};
  };
  auto fallbackSampleRate() -> int override { return 48000; };

  auto supportedBitDepths() -> std::vector<int> override { return {16}; };
  auto fallbackBitDepth() -> int override { return 16; };
};

#endif  // AUDIO_CODEC_MP3_H