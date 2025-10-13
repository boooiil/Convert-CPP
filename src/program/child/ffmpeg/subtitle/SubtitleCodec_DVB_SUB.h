#ifndef SUBTITLE_CODEC_DVB_SUB_H
#define SUBTITLE_CODEC_DVB_SUB_H

#include "../../../settings/enums/SubtitleCodec.h"
#include "BaseSubtitleCodec.h"
#include <set>

class SubtitleCodec_DVB_SUB : public BaseSubtitleCodec {
public:
  auto getType() -> SubtitleCodec override { return SubtitleCodec::DVB_SUB; }
  auto getName() -> std::string override { return "dvb_sub"; }
  auto getAliases() -> std::set<std::string> override { return {"dvb_sub"}; }
};

#endif // SUBTITLE_CODEC_DVB_SUB_H