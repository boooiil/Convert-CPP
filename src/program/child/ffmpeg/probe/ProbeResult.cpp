#include "ProbeResult.h"

#include <nlohmann/json.hpp>

#include "../../../../utils/logging/Logger.h"
#include "ProbeResultFormat.h"
#include "ProbeResultStreamAudio.h"
#include "ProbeResultStreamSubtitle.h"
#include "ProbeResultStreamVideo.h"

ProbeResult::ProbeResult() {};
ProbeResult::~ProbeResult() { LOG_DEBUG("Deconstructing ProbeResult"); };

ProbeResult::ProbeResult(nlohmann::json JSON) {
  ProbeResult::format = ProbeResultFormat(JSON["format"]);

  for (nlohmann::json stream : JSON["streams"]) {
    if (stream["codec_type"] == "video") {
      ProbeResult::videoStreams.push_back(ProbeResultStreamVideo(stream));
    } else if (stream["codec_type"] == "audio") {
      ProbeResult::audioStreams.push_back(ProbeResultStreamAudio(stream));
    } else if (stream["codec_type"] == "subtitle") {
      ProbeResult::subtitleStreams.push_back(ProbeResultStreamSubtitle(stream));
    }
  }
}