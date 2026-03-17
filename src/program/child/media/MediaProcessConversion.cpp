#include "MediaProcessConversion.h"

#include <regex>
#include <stdexcept>
#include <string>

#include "../../../utils/RegexUtils.h"
#include "../../../utils/StringUtils.h"
#include "../../../utils/logging/Logger.h"
#include "../../settings/enums/Activity_N.h"
#include "../../settings/enums/Encoders_N.h"
#include "Media.h"
#include "MediaProcess.h"
#include "src/utils/logging/Logger.h"

MediaProcessConversion::MediaProcessConversion(Media *media)
    : MediaProcess(media) {}

MediaProcessConversion::~MediaProcessConversion() {
  MediaProcess::~MediaProcess();
}

void MediaProcessConversion::parse(std::string data) {
  LOG_VERBOSE("PARSING LINE:", data);

  ChildOptions &childOptions = this->object->getOptions();
  // ArgumentRegistry& argumentRegistry = *childOptions.argumentRegistry;

  // wants to do a stress test
  // wants to know if virus or blockage
  // kidneys failing
  // she is anemic

  // Return fail IF:
  // 1&2. Encode fails to find a device
  // 3. Encode fails to load nvcuda.dll
  // 4. Encode fails to find an nvidia device
  if (RegexUtils::isMatch(data, "openencodesessionex failed: out of memory",
                          std::regex::icase) ||
      RegexUtils::isMatch(data, "-capable device is detected",
                          std::regex::icase) ||
      RegexUtils::isMatch(data, "cannot load nvcuda.dll", std::regex::icase) ||
      RegexUtils::isMatch(data, "device type cuda needed for codec",
                          std::regex::icase)) {
    // if the user wants to use hardware encoding (nvenc, amf, qsv)
    LOG_DEBUG(
        "User selected hardware encoding but no compatible device was found.");
    LOG_DEBUG("Wanted encoder:",
              Encoders_N::definition(childOptions.runningEncoder));

    // this does not make sense to keep
    // if the user specified a hw encoder then it should be assumed that
    // they want hardware encoding.
    // FlagArgument &hweFlag =
    // *childOptions.argumentRegistry->get_t<FlagArgument>(
    //     Command_N::HARDWAREENCODE);

    // if (hweFlag) {
    //   this->object->setActivity(Activity_N::FAILED_HARDWARE);
    //   return;
    // }

    switch (childOptions.runningEncoder) {
    case Encoders_N::AV1_AMF:
    case Encoders_N::AV1_NVENC:
    case Encoders_N::AV1_QSV:
    case Encoders_N::H264_AMF:
    case Encoders_N::H264_NVENC:
    case Encoders_N::H264_QSV:
    case Encoders_N::HEVC_AMF:
    case Encoders_N::HEVC_NVENC:
    case Encoders_N::HEVC_QSV:
      this->object->setActivity(Activity_N::FAILED_HARDWARE);
      break;
    default:
      throw std::runtime_error(
          "Out of memory even though hardware encoding is disabled. This "
          "should not happen.");
    };
  }

  // If the file is already encoded, set the process status to validating
  else if (RegexUtils::isMatch(data, "already exists", std::regex::icase)) {
    this->object->setActivity(Activity_N::WAITING_VALIDATE);
  } else if (RegexUtils::isMatch(data, "no such file", std::regex::icase)) {
    this->object->setActivity(Activity_N::FAILED_FILE_MISSING);
  } else if (RegexUtils::isMatch(data, "matches no streams",
                                 std::regex::icase)) {
    this->object->setActivity(Activity_N::FAILED_INVALID_AUDIO_STREAMS);
  } else if (RegexUtils::isMatch(data, "Invalid duration for option ss")) {
    this->object->setActivity(Activity_N::FAILED_INVALID_DURATION_SS);
  } else if (RegexUtils::isMatch(data, "Invalid duration for option to")) {
    this->object->setActivity(Activity_N::FAILED_INVALID_DURATION_TO);
  } else if (RegexUtils::isMatch(data, "Unknown encoder")) {
    this->object->setActivity(Activity_N::FAILED_INVALID_ENCODER);
  } else if (StringUtils::contains(data, "Rematrix is needed between") ||
             StringUtils::contains(data, "Failed to configure output pad") ||
             StringUtils::contains(data, "Error reinitializing filters")) {
    this->object->setActivity(Activity_N::FAILED_INVALID_AUDIO_CHANNELS);
  } else if (StringUtils::contains(data,
                                   "from text to text or bitmap to bitmap")) {
    this->object->setActivity(Activity_N::FAILED_SUBTITLE_ENCODING_INVALID);
  }

  else if (RegexUtils::isMatch(data, "frame=\\s*(\\d+)")) {
    LOG_DEBUG("Line matched for progress parsing.");

    std::string quality =
        RegexUtils::getFirstMatch(data, "q=(\\d+\\.\\d+|-\\d+\\.\\d+)");
    std::string bitrate =
        RegexUtils::getFirstMatch(data, "bitrate=\\s*(\\d+\\.\\d+)");
    std::string completedFrames =
        RegexUtils::getFirstMatch(data, "frame=\\s*(\\d+)");
    std::string fps =
        RegexUtils::getFirstMatch(data, "fps=\\s*(\\d+\\.\\d+|\\d+)");
    ;

    if (quality == "")
      quality = "-1.0";
    if (bitrate == "")
      bitrate = "-1.0";
    if (completedFrames == "")
      completedFrames = "0";
    if (fps == "")
      fps = "-1.0";

    // assert(quality != "");
    // assert(bitrate != "");
    // assert(size != "");
    // assert(completedFrames != "");
    // assert(fps != "");

    if (quality != "-1.0") {
      this->object->getFile().processing_info.quality = std::stof(quality);
    }
    this->object->getFile().processing_info.bitrate = std::stof(bitrate);
    this->object->getFile().processing_info.completedFrames =
        std::stoll(completedFrames);
    this->object->getFile().processing_info.fps = std::stof(fps);

    LOG_DEBUG("QUALITY:", quality);
    LOG_DEBUG("BITRATE:", bitrate);
    LOG_DEBUG("COMPLETED FRAMES:", completedFrames);
    LOG_DEBUG("FPS:", fps);
  }

  LOG_DEBUG("PARSING END");
}