#include "ProbeResult.h"

#include <nlohmann/json.hpp>

#include "../../../../utils/logging/Logger.h"
#include "ProbeResultFormat.h"
#include "ProbeResultStreamAudio.h"
#include "ProbeResultStreamSubtitle.h"
#include "ProbeResultStreamVideo.h"
#include "src/utils/logging/LogColor.h"
#include "src/utils/logging/Logger.h"

ProbeResult::ProbeResult() {};
ProbeResult::~ProbeResult() { LOG_DEBUG("Deconstructing ProbeResult"); };

ProbeResult::ProbeResult(nlohmann::json JSON) {
  ProbeResult::format = ProbeResultFormat(JSON["format"]);

  for (nlohmann::json stream : JSON["streams"]) {
    if (stream["codec_type"] == "video") {
      LOG_DEBUG("Getting video stream...");
      ProbeResult::videoStreams.push_back(ProbeResultStreamVideo(stream));
      LOG_DEBUG("... Done getting video stream.");
    } else if (stream["codec_type"] == "audio") {
      LOG_DEBUG("Getting audio stream...");
      ProbeResult::audioStreams.push_back(ProbeResultStreamAudio(stream));
      LOG_DEBUG("... Done getting audio stream.");
    } else if (stream["codec_type"] == "subtitle") {
      LOG_DEBUG("Getting subtitle stream...");
      ProbeResult::subtitleStreams.push_back(ProbeResultStreamSubtitle(stream));
      LOG_DEBUG("... Done getting subtitle stream.");
    } else if (stream["codec_type"] == "attachment") {
      LOG_DEBUG("Getting attachment stream...");
      ProbeResult::attachmentStreams.push_back(
          ProbeResultStreamAttachment(stream));
      LOG_DEBUG("... Done getting attachment stream.");
    } else {
      LOG_DEBUG(LogColor::fgRed("Unknown stream type: " +
                                std::string(stream["codec_type"])));
    }
  }
}