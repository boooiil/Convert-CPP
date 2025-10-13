#ifndef SUBTITLE_CODEC_SSA_H
#define SUBTITLE_CODEC_SSA_H

#include "../../../settings/enums/SubtitleCodec.h"
#include "BaseSubtitleCodec.h"
#include <set>

class SubtitleCodec_SSA : public BaseSubtitleCodec {
public:
  auto getType() -> SubtitleCodec override { return SubtitleCodec::SSA; }
  auto getName() -> std::string override { return "ssa"; }
  auto getAliases() -> std::set<std::string> override { return {"ssa"}; }
};

#endif // SUBTITLE_CODEC_SSA_H