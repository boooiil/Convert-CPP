#ifndef SUBTITLE_CODEC_H
#define SUBTITLE_CODEC_H

#include <string>
#include <vector>

#include "StringEnumDataHolder.h"

enum class SubtitleCodec {
  INVALID = 0,

  NONE,
  ASS,
  SRT,
  SSA,
  SUBRIP,
  DVB_SUB,
  WEBVTT,
  MOV_TEXT
  // public:
  //  static const SubtitleCodec NONE;      // default
  //  static const SubtitleCodec ASS;       // mkv default
  //  static const SubtitleCodec SRT;       // mkv
  //  static const SubtitleCodec SSA;       // mkv
  //  static const SubtitleCodec SUBRIP;    // ??
  //  static const SubtitleCodec DVB_SUB;   // ??
  //  static const SubtitleCodec WEBVTT;    // ??
  //  static const SubtitleCodec MOV_TEXT;  // mp4, mov

  // static const std::vector<const SubtitleCodec*> _all() {
  //   return {
  //       &NONE, &ASS, &SRT, &SSA, &SUBRIP, &DVB_SUB, &WEBVTT, &MOV_TEXT,
  //   };
  // };

  // static const SubtitleCodec getKey(std::string value) {
  //   for (auto& item : _all()) {
  //     if (item->getName() == value) {
  //       return *item;
  //     }
  //   }
  //   return NONE;
  // }
};

#endif  // SUBTITLE_CODEC_H