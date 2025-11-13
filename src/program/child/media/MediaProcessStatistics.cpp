#include "MediaProcessStatistics.h"

#include <assert.h>
#include <stdio.h>

#include <array>
#include <cmath>
#include <cstdio>
#include <exception>
#include <iosfwd>
#include <memory>
#include <nlohmann/json.hpp>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

#include "../../../utils/ListUtils.h"
#include "../../../utils/RegexUtils.h"
#include "../../../utils/logging/Logger.h"
#include "../../Program.h"
#include "../../settings/arguments/video/Quality.h"
#include "../../settings/enums/Activity_N.h"
#include "../ffmpeg/probe/ProbeResult.h"
#include "../ffmpeg/probe/ProbeResultStreamVideo.h"
#include "Media.h"
#include "MediaFormat.h"
#include "MediaProcess.h"

#ifdef _WIN32
#define popen _popen
#define pclose _pclose
#endif
MediaProcessStatistics::MediaProcessStatistics(Media *media)
    : MediaProcess(media) {}

MediaProcessStatistics::~MediaProcessStatistics() {
  MediaProcess::~MediaProcess();
}

void MediaProcessStatistics::start(std::string command) {
  LOG_DEBUG("SENDING COMMAND:", command);

  MediaProcessStatistics::status = MediaProcessStatistics::Status::RUNNING;

  std::array<char, 128> buffer;
  std::string result;

  // Open pipe to file
  std::unique_ptr<FILE, decltype(&pclose)> pipe(popen(command.c_str(), "r"),
                                                pclose);
  if (!pipe) {
    throw std::runtime_error("popen() failed!");
  }

  // TODO: this might cause problems in the future

  // Read from pipe
  while (fgets(buffer.data(), static_cast<int>(buffer.size()), pipe.get()) !=
         nullptr) {
    // MediaProcessStatistics::container.log.debug(
    //     {"RAW OUTPUT: ", buffer.data()});
    result += buffer.data();
    // std::this_thread::sleep_for(
    //     std::chrono::seconds(1));  // delay for long running processes
  }

  // MediaProcessStatistics::container.log.debug(
  //    {"OUTPUT: ", result});
  MediaProcessStatistics::parse(result);
}

void MediaProcessStatistics::parse(std::string data) {
  try {
    if (RegexUtils::isMatch(data, "not recognized") ||
        RegexUtils::isMatch(data, "unknown command")) {
      LOG_DEBUG("Could not find ffprobe, failing.", data);
      // TODO: add FAILED_MISSING_FFPROBE
      this->object->setActivity(Activity_N::FAILED);
      return;
    }

    nlohmann::json JSON = nlohmann::json::parse(data);
    this->object->probeResult = new ProbeResult(JSON);

    // TODO: validate that file exists, assert fails when file missing
    assert(this->object->probeResult->videoStreams.size() > 0);

    LOG_DEBUG("VIDEO STREAMS: ",
              std::to_string(this->object->probeResult->videoStreams.size()));

    ProbeResultStreamVideo prsv = this->object->probeResult->videoStreams[0];

    std::istringstream rateStream(
        this->object->probeResult->videoStreams[0].r_frame_rate);

    int numerator, denominator;
    char slash;
    rateStream >> numerator >> slash >> denominator;

    int hours;
    int minutes;
    int seconds;
    int duration;

    if (!prsv.tags.DURATION.empty()) {
      // TODO: calc duration??
      std::vector<std::string> timeParts =
          ListUtils::splitv(prsv.tags.DURATION, std::string(":"));

      hours = std::stoi(timeParts[0]);
      minutes = std::stoi(timeParts[1]);
      seconds = std::stoi(timeParts[2]);
      duration = (hours * 60 * 60) + (minutes * 60) + seconds;
    } else {
      LOG_DEBUG("DURATION NOT FOUND");
      LOG_DEBUG("Obtaining duration from format. This "
                "could be inaccurate.");

      duration = (int)std::stof(this->object->probeResult->format.duration);
    }

    this->object->file->size =
        std::stoull(this->object->probeResult->format.size);
    this->object->video->fps =
        round((static_cast<float>(numerator) / denominator) * 100) / 100;
    this->object->video->width = prsv.width;
    this->object->video->height = prsv.height;
    this->object->video->totalFrames = static_cast<unsigned long long>(
        std::ceil(duration * this->object->video->fps));

    // assert(!settings->argumentParser->quality.get().name.empty());

    ChildOptions &childOptions =
        *Program::settings->childOptionsMap[this->object->id];
    ArgumentRegistry &argumentRegistry = *childOptions.argumentRegistry;
    MediaFormat format =
        argumentRegistry.get_t<Quality>(Command_N::QUALITY)->get();

    this->object->video->convertedWidth = std::to_string(format.width);
    this->object->video->convertedHeight = std::to_string(format.getResolution(
        this->object->video->width, this->object->video->height, format.width));
    this->object->video->convertedResolution =
        this->object->video->convertedWidth + ":" +
        this->object->video->convertedHeight;
    this->object->video->crf = format.crf;
  } catch (const std::exception &e) {
    LOG_DEBUG("ERROR: ", e.what());
    this->object->setActivity(Activity_N::FAILED_JSON_PARSE);
    MediaProcessStatistics::status = MediaProcess::Status::_ERROR;
    return;
  }
}