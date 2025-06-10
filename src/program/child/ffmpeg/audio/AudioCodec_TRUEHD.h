#ifndef AUDIO_CODEC_TRUEHD_H
#define AUDIO_CODEC_TRUEHD_H

#include "../../../settings/enums/AudioCodec.h"
#include "../../../settings/enums/StringEnumDataHolder.h"
#include "BaseAudioCodec.h"

class AudioCodec_TRUEHD : public BaseAudioCodec {
 public:
  auto getType() -> AudioCodec override { return AudioCodec::TRUEHD; };

  auto supportedChannels() -> std::vector<int> override {
    return {
        1,  // mono
        2,  // l, r
        3,  // fl, c, fr
        4,  // fl, fr, rl, rr
        5,  // fl, fc, fr, rl, rr
        6,  // fl, fc, fr, rl, rr, lfe
        7,  // fl, fc, fr, rl, rr, lfe, rc
        8,  // fl, fc, fr, rl, rr, lfe, sl, sr
        9,  // fl, fc, fr, rl, rr, lfe, sl, sr, rc
        10  // fl, fc, fr, rl, rr, lfe, sl, sr, rc, tl, tr
    };
  };
  auto fallbackChannel() -> int override { return 2; };

  auto supportedSampleRates() -> std::vector<int> override {
    return {
        32000,   // 32 kHz
        44100,   // 44.1 kHz
        48000,   // 48 kHz
        96000,   // 96 kHz
        192000,  // 192 kHz
        384000   // 384 kHz
    };
  };
  auto fallbackSampleRate() -> int override { return 48000; };

  auto supportedBitDepths() -> std::vector<int> override { return {16, 24}; };
  auto fallbackBitDepth() -> int override { return 16; };
};

#endif  // AUDIO_CODEC_TRUEHD_H