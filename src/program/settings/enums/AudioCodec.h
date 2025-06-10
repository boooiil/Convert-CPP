#ifndef AUDIO_CODEC_H
#define AUDIO_CODEC_H

#include <string>
#include <vector>

#include "StringEnumDataHolder.h"

enum class AudioCodec {
  INVALID = 0,

  NONE,
  AAC,
  AC3,
  EAC3,
  MP3,
  FLAC,
  TRUEHD,
  VORBIS,
  OPUS
  //public:
  // static const StringEnumDataHolder<AudioCodec> NONE;
  // static const StringEnumDataHolder<AudioCodec> AAC;  // mp4 default
  // static const StringEnumDataHolder<AudioCodec> AC3;  // mp4, mkv
  // static const StringEnumDataHolder<AudioCodec> EAC3;
  // static const StringEnumDataHolder<AudioCodec> MP3;   // avi default
  // static const StringEnumDataHolder<AudioCodec> FLAC;  // mkv
  // static const StringEnumDataHolder<AudioCodec> TRUEHD;
  // static const StringEnumDataHolder<AudioCodec>
  //     VORBIS;                                          // mkv default (use aac)
  // static const StringEnumDataHolder<AudioCodec> OPUS;  // mkv

  // static const std::vector<const StringEnumDataHolder<AudioCodec>*> _all() {
  //   return {&NONE, &AAC, &AC3, &EAC3, &MP3, &FLAC, &TRUEHD, &VORBIS, &OPUS};
  // };

  // static const StringEnumDataHolder<AudioCodec> getKey(std::string value) {
  //   for (auto& item : _all()) {
  //     if (item->getName() == value) {
  //       return *item;
  //     }
  //   }
  //   return NONE;
  // }
};

#endif  // AUDIO_CODEC_H