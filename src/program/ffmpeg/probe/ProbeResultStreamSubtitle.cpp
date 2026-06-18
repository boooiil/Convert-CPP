#include "ProbeResultStreamSubtitle.h"

#include <nlohmann/json.hpp>

#include "src/utils/logging/Logger.h"

/**
 * TODO: Validate ffmpeg verison to avoid missing fields
 */

ProbeResultStreamSubtitle::ProbeResultStreamSubtitle(nlohmann::json JSON)
    : ProbeResultStream(JSON), duration_ts(-1) {
  LOG_DEBUG(Logger::Priority::INFO, "Starting "
                                    "ProbeResultStreamSubtitle...");

  /* Does not appear on linux (ubuntu), apt, ffmpeg=4.4.2-0ubuntu0.22.04.1
   */
  // ProbeResultStreamSubtitle::extradata_size = JSON["extradata_size"];

  if (!JSON["duration_ts"].is_null()) {
    LOG_VERBOSE(Logger::Priority::INFO,
                "duration_ts: ", std::to_string((int)JSON["duration_ts"]));
    ProbeResultStreamSubtitle::duration_ts = JSON["duration_ts"];
  }
  if (!JSON["duration"].is_null()) {
    LOG_VERBOSE(Logger::Priority::INFO, "duration: ", JSON["duration"]);
    ProbeResultStreamSubtitle::duration = JSON["duration"];
  }
  LOG_DEBUG(Logger::Priority::INFO, "Ended ProbeResultStreamSubtitle");
}