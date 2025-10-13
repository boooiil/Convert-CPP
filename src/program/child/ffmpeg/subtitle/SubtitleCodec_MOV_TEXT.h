#ifndef SUBTITLE_CODEC_MOV_TEXT_H
#define SUBTITLE_CODEC_MOV_TEXT_H

#include "../../../settings/enums/SubtitleCodec.h"
#include "BaseSubtitleCodec.h"
#include <set>

class SubtitleCodec_MOV_TEXT : public BaseSubtitleCodec {
public:
  auto getType() -> SubtitleCodec override { return SubtitleCodec::MOV_TEXT; }
  auto getName() -> std::string override { return "mov_text"; }
  auto getAliases() -> std::set<std::string> override {
    return {"mov_text", "subrip", "srt"};
  }
};

#endif // SUBTITLE_CODEC_MOV_TEXT_H