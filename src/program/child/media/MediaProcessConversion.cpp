#include "MediaProcessConversion.h"

#include <regex>
#include <stdexcept>
#include <string>

#include "../../../utils/logging/Logger.h"
#include "../../../utils/RegexUtils.h"
#include "../../../utils/StringUtils.h"
#include "../../Program.h"
#include "../../settings/arguments/ArgumentRegistry.h"
#include "../../settings/arguments/FlagArgument.h"
#include "../../settings/enums/Activity.h"
#include "../../settings/enums/Encoders.h"
#include "../../settings/Settings.h"
#include "Media.h"
#include "MediaProcess.h"

MediaProcessConversion::MediaProcessConversion(Media* media)
  : MediaProcess(media) {
}

MediaProcessConversion::~MediaProcessConversion() {
  MediaProcess::~MediaProcess();
}

void MediaProcessConversion::parse(std::string data) {
  LOG_DEBUG("PARSING LINE:", data);

  ChildOptions& childOptions = *Program::settings->childOptionsMap[this->object->id];
  ArgumentRegistry& argumentRegistry = *childOptions.argumentRegistry;

  // Return fail IF:
  // 1&2. Encode fails to find a device
  // 3. Encode fails to load nvcuda.dll
  // 4. Encode fails to find an nvidia device
  if (RegexUtils::isMatch(data, "openencodesessionex failed: out of memory",
    std::regex::icase) ||
    RegexUtils::isMatch(data, "no capable devices found",
      std::regex::icase) ||
    RegexUtils::isMatch(data, "cannot load nvcuda.dll", std::regex::icase) ||
    RegexUtils::isMatch(data, "device type cuda needed for codec",
      std::regex::icase)) {
    // if the user wants to use hardware encoding (nvenc, amf, qsv)

    FlagArgument& hweFlag = *Program::settings->childOptionsMap[this->object->id]->argumentRegistry->get_t<FlagArgument>(Command::HARDWAREENCODE);


    if (hweFlag) {
      this->object->setActivity(Activity::FAILED_HARDWARE);
      return;
    }

    switch (childOptions.runningEncoder) {
    case Encoders::AV1_AMF:
    case Encoders::AV1_NVENC:
    case Encoders::AV1_QSV:
    case Encoders::H264_AMF:
    case Encoders::H264_NVENC:
    case Encoders::H264_QSV:
    case Encoders::HEVC_AMF:
    case Encoders::HEVC_NVENC:
    case Encoders::HEVC_QSV:
      this->object->setActivity(Activity::FAILED_HARDWARE);
      break;
    default:
      throw std::runtime_error(
        "Out of memory even though hardware encoding is disabled. This "
        "should not happen.");

    };
  }

  // If the file is already encoded, set the process status to validating
  else if (RegexUtils::isMatch(data, "already exists", std::regex::icase)) {
    this->object->setActivity(Activity::WAITING_VALIDATE);
  }
  else if (RegexUtils::isMatch(data, "no such file", std::regex::icase)) {
    this->object->setActivity(Activity::FAILED_FILE_MISSING);
  }
  else if (RegexUtils::isMatch(data, "matches no streams",
    std::regex::icase)) {
    this->object->setActivity(Activity::FAILED_INVALID_AUDIO_STREAMS);
  }
  else if (RegexUtils::isMatch(data, "Invalid duration for option ss")) {
    this->object->setActivity(Activity::FAILED_INVALID_DURATION_SS);
  }
  else if (RegexUtils::isMatch(data, "Invalid duration for option to")) {
    this->object->setActivity(Activity::FAILED_INVALID_DURATION_TO);
  }
  else if (RegexUtils::isMatch(data, "Unknown encoder")) {
    this->object->setActivity(Activity::FAILED_INVALID_ENCODER);
  }
  else if (StringUtils::contains(data, "Rematrix is needed between") ||
    StringUtils::contains(data, "Failed to configure output pad") ||
    StringUtils::contains(data, "Error reinitializing filters")) {
    this->object->setActivity(Activity::FAILED_INVALID_AUDIO_CHANNELS);
  }

  else if (RegexUtils::isMatch(data, "frame=.+?(\\d+)")) {
    LOG_DEBUG("PARSING LINE");

    std::string quality =
      RegexUtils::getFirstMatch(data, "q=(\\d+\\.\\d+|-\\d+\\.\\d+)");
    std::string bitrate =
      RegexUtils::getFirstMatch(data, "bitrate= ?(\\d+\\.\\d+)");
    std::string completedFrames =
      RegexUtils::getFirstMatch(data, "frame=.+?(\\d+)");
    std::string fps =
      RegexUtils::getFirstMatch(data, "fps=.+?(\\d+\\.\\d+|\\d+)");
    ;

    if (quality == "") quality = "-1.0";
    if (bitrate == "") bitrate = "-1.0";
    if (completedFrames == "") completedFrames = "-1";
    if (fps == "") fps = "-1.0";

    // assert(quality != "");
    // assert(bitrate != "");
    // assert(size != "");
    // assert(completedFrames != "");
    // assert(fps != "");

    if (quality != "-1.0") this->object->working->quality = std::stof(quality);
    this->object->working->bitrate = std::stof(bitrate);
    this->object->working->completedFrames = std::stoll(completedFrames);
    this->object->working->fps = std::stof(fps);

    LOG_DEBUG("QUALITY:", quality);
    LOG_DEBUG("BITRATE:", bitrate);
    LOG_DEBUG("COMPLETED FRAMES:", completedFrames);
    LOG_DEBUG("FPS:", fps);
  }

  LOG_DEBUG("PARSING END");
}