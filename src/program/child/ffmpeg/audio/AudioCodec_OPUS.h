#ifndef AUDIO_CODEC_OPUS_H
#define AUDIO_CODEC_OPUS_H

#include "../../../settings/enums/AudioCodec.h"
#include "../../../settings/enums/StringEnumDataHolder.h"
#include "BaseAudioCodec.h"

class AudioCodec_OPUS : public BaseAudioCodec {
 public:
  auto getType() -> AudioCodec override { return AudioCodec::OPUS; };

  auto supportedChannels() -> std::vector<int> override {
    return {
        1,  // mono
        2,  // l, r
        3,  // fl, c, fr
        4,  // fl, fr, rl, rr
        5,  // fl, fc, fr, rl, rr
        6,  // fl, fc, fr, rl, rr, lfe
        7,  // fl, fc, fr, rl, rr, lfe, rc
        8   // fl, fc, fr, rl, rr, lfe, sl, sr
    };
  };
  auto fallbackChannel() -> int override { return 2; };

  auto supportedSampleRates() -> std::vector<int> override {
    return {8000, 12000, 16000, 24000, 48000};
  };
  auto fallbackSampleRate() -> int override { return 48000; };

  auto supportedBitDepths() -> std::vector<int> override { return {16}; };
  auto fallbackBitDepth() -> int override { return 16; };
};

#endif  // AUDIO_CODEC_OPUS_H