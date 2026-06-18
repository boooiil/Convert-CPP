#include "MediaWorkingProperties.h"

#include "../../../utils/logging/Logger.h"

MediaWorkingProperties::MediaWorkingProperties()
    : quality(0.0), bitrate(0.0), fps(0.0), completedFrames(0) {}

MediaWorkingProperties::~MediaWorkingProperties() {
  LOG_DEBUG(Logger::Priority::INFO, "Deconstructing MediaWorkingProperties");
}