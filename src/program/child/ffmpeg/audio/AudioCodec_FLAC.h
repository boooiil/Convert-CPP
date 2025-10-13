#ifndef AUDIO_CODEC_FLAC_H
#define AUDIO_CODEC_FLAC_H

#include "../../../settings/enums/AudioCodec.h"
#include "BaseAudioCodec.h"

class AudioCodec_FLAC : public BaseAudioCodec {
public:
  AudioCodec_FLAC(int channel = 2, int sampleRate = 96000, int bitDepth = 24)
      : BaseAudioCodec(channel, sampleRate, bitDepth) {};

  auto getType() -> AudioCodec override { return AudioCodec::FLAC; };
  auto getName() -> std::string override { return "flac"; };
  auto getDisplayName() -> std::string override { return "FLAC"; };
  auto getAliases() -> std::set<std::string> override { return {"flac"}; };

  auto supportedChannels() -> std::vector<int> override {
    return {
        1, // mono
        2, // l, r
        3, // fl, c, fr
        4, // fl, fr, rl, rr
        5, // fl, fc, fr, rl, rr
        6, // fl, fc, fr, rl, rr, lfe
        7, // fl, fc, fr, rl, rr, lfe, rc
        8, // fl, fc, fr, rl, rr, lfe, sl, sr
        9, // fl, fc, fr, rl, rr, lfe, sl, sr, rc
        10 // fl, fc, fr, rl, rr, lfe, sl, sr, rc, tl, tr
    };
  };
  auto fallbackChannel() -> int override { return 2; };

  auto supportedSampleRates() -> std::vector<int> override {
    return {8000,  11025, 12000, 16000, 22050,  24000,  32000, 44100,
            48000, 64000, 88200, 96000, 176400, 192000, 384000};
  };
  auto fallbackSampleRate() -> int override { return 96000; };

  auto supportedBitDepths() -> std::vector<int> override {
    return {8, 16, 24, 32};
  };
  auto fallbackBitDepth() -> int override { return 24; };
};

#endif // AUDIO_CODEC_FLAC_H