#include "ProbeResultStreamAttachment.h"
#include "ProbeResultStream.h"
#include "ProbeResultStreamDisposition.h"
#include "ProbeResultStreamTags.h"
#include "src/utils/logging/Logger.h"
#include <nlohmann/json.hpp>

ProbeResultStreamAttachment::ProbeResultStreamAttachment(nlohmann::json JSON)
    : ProbeResultStream(JSON) {

  LOG_DEBUG("Starting ProbeResultStreamAttachment...");

  if (!JSON["duration_ts"].is_null()) {
    this->duration_ts = JSON["duration_ts"];
  }

  if (!JSON["duration"].is_null()) {
    this->duration = JSON["duration"];
  }

  if (!JSON["disposition"].is_null()) {
    this->disposition = ProbeResultStreamDisposition(JSON["disposition"]);
  }

  if (!JSON["tags"].is_null()) {
    this->tags = ProbeResultStreamTags(JSON["tags"]);
  }
}