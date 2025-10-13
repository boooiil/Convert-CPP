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
#include <functional>
#include <string>
#include <unordered_map>

class AudioCodecFactory {
public:
  static auto create(AudioCodec audioCodec) -> BaseAudioCodec * {
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
  static auto create(const std::string &audioCodecName) -> BaseAudioCodec * {
    if (audioCodecName == "aac") {
      return new AudioCodec_AAC();
    } else if (audioCodecName == "ac3") {
      return new AudioCodec_AC3();
    } else if (audioCodecName == "eac3") {
      return new AudioCodec_EAC3();
    } else if (audioCodecName == "flac") {
      return new AudioCodec_FLAC();
    } else if (audioCodecName == "mp3") {
      return new AudioCodec_MP3();
    } else if (audioCodecName == "opus") {
      return new AudioCodec_OPUS();
    } else if (audioCodecName == "truehd") {
      return new AudioCodec_TRUEHD();
    } else if (audioCodecName == "vorbis") {
      return new AudioCodec_VORBIS();
    }
    return nullptr;
  }

private:
  static auto initialize() -> void {
    auto aac = []() -> BaseAudioCodec * { return new AudioCodec_AAC(); };
    for (auto &alias : AudioCodec_AAC().getAliases()) {
      codec_map[alias] = aac;
    }

    auto ac3 = []() -> BaseAudioCodec * { return new AudioCodec_AC3(); };
    for (auto &alias : AudioCodec_AC3().getAliases()) {
      codec_map[alias] = ac3;
    }

    auto eac3 = []() -> BaseAudioCodec * { return new AudioCodec_EAC3(); };
    for (auto &alias : AudioCodec_EAC3().getAliases()) {
      codec_map[alias] = eac3;
    }

    auto flac = []() -> BaseAudioCodec * { return new AudioCodec_FLAC(); };
    for (auto &alias : AudioCodec_FLAC().getAliases()) {
      codec_map[alias] = flac;
    }

    auto mp3 = []() -> BaseAudioCodec * { return new AudioCodec_MP3(); };
    for (auto &alias : AudioCodec_MP3().getAliases()) {
      codec_map[alias] = mp3;
    }

    auto opus = []() -> BaseAudioCodec * { return new AudioCodec_OPUS(); };
    for (auto &alias : AudioCodec_OPUS().getAliases()) {
      codec_map[alias] = opus;
    }

    auto truehd = []() -> BaseAudioCodec * { return new AudioCodec_TRUEHD(); };
    for (auto &alias : AudioCodec_TRUEHD().getAliases()) {
      codec_map[alias] = truehd;
    }

    auto vorbis = []() -> BaseAudioCodec * { return new AudioCodec_VORBIS(); };
    for (auto &alias : AudioCodec_VORBIS().getAliases()) {
      codec_map[alias] = vorbis;
    }
  }

  static std::unordered_map<std::string, std::function<BaseAudioCodec *()>>
      codec_map;
};

#endif // AUDIO_CODEC_FACTORY_H
