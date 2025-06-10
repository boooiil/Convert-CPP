#ifndef AUDIO_CODEC_FACTORY_H
#define AUDIO_CODEC_FACTORY_H

#include "../../../settings/enums/AudioCodec.h"
#include "AudioCodec_AAC.h"
#include "AudioCodec_AC3.h"
#include "AudioCodec_EAC3.h"
#include "AudioCodec_FLAC.h"
#include "AudioCodec_MP3.h"
#include "AudioCodec_OPUS.h"
#include "AudioCodec_TRUEHD.h"
#include "AudioCodec_VORBIS.h"
#include "BaseAudioCodec.h"

class AudioCodecFactory {
 public:
  auto createAudioCodec(AudioCodec audioCodec) -> BaseAudioCodec* {
    switch (audioCodec) {
      case AudioCodec::AAC:
        return new AudioCodec_AAC();
      case AudioCodec::AC3:
        return new AudioCodec_AC3();
      case AudioCodec::EAC3:
        return new AudioCodec_EAC3();
      case AudioCodec::FLAC:
        return new AudioCodec_FLAC();
      case AudioCodec::MP3:
        return new AudioCodec_MP3();
      case AudioCodec::OPUS:
        return new AudioCodec_OPUS();
      case AudioCodec::TRUEHD:
        return new AudioCodec_TRUEHD();
      case AudioCodec::VORBIS:
        return new AudioCodec_VORBIS();
      case AudioCodec::INVALID:
        [[fallthrough]];
      case AudioCodec::NONE:
        return nullptr;
    }
  }
};

#endif  // AUDIO_CODEC_FACTORY_H