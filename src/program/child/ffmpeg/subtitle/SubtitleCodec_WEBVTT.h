#ifndef SUBTITLE_CODEC_WEBVTT_H
#define SUBTITLE_CODEC_WEBVTT_H

#include "../../../settings/enums/SubtitleCodec.h"
#include "BaseSubtitleCodec.h"
#include <set>

class SubtitleCodec_WEBVTT : public BaseSubtitleCodec {
public:
  auto getType() -> SubtitleCodec override { return SubtitleCodec::WEBVTT; }
  auto getName() -> std::string override { return "webvtt"; }
  auto getAliases() -> std::set<std::string> override {
    return {"webvtt", "vtt"};
  }
};

#endif // SUBTITLE_CODEC_WEBVTT_H