#include "ProbeResultStreamAttachment.h"
#include "ProbeResultStream.h"
#include "src/utils/logging/Logger.h"
#include <nlohmann/json.hpp>

ProbeResultStreamAttachment::ProbeResultStreamAttachment(nlohmann::json JSON)
    : ProbeResultStream(JSON) {

  LOG_DEBUG(Logger::Priority::INFO, "Starting ProbeResultStreamAttachment...");

  if (!JSON["duration_ts"].is_null()) {
    this->duration_ts = JSON["duration_ts"];
  }

  if (!JSON["duration"].is_null()) {
    this->duration = JSON["duration"];
  }
}