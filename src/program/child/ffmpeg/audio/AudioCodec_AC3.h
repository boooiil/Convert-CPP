#ifndef AUDIO_CODEC_AC3_H
#define AUDIO_CODEC_AC3_H

#include "../../../settings/enums/AudioCodec.h"
#include "BaseAudioCodec.h"

class AudioCodec_AC3 : public BaseAudioCodec {
public:
  AudioCodec_AC3(int channel = 2, int sampleRate = 48000, int bitDepth = 16)
      : BaseAudioCodec(channel, sampleRate, bitDepth) {};

  auto getType() -> AudioCodec override { return AudioCodec::AC3; };
  auto getName() -> std::string override { return "ac3"; };
  auto getDisplayName() -> std::string override { return "Dolby Digital"; };
  auto getAliases() -> std::set<std::string> override {
    return {"ac3", "dolby_digital"};
  };

  auto supportedChannels() -> std::vector<int> override {
    return {
        1, // mono
        2, // l, r
        3, // fl, c, fr
        4, // fl, fr, rl, rr
        5, // fl, fc, fr, rl, rr
        6, // fl, fc, fr, rl, rr, lfe
        7, // fl, fc, fr, rl, rr, lfe, rc
        8  // fl, fc, fr, rl, rr, lfe, sl, sr
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

#endif // AUDIO_CODEC_AC3_H