#include "MediaProcessValidate.h"

#include <regex>
#include <string>

#include "../../../utils/RegexUtils.h"
#include "../../settings/enums/Activity.h"
#include "Media.h"
#include "MediaProcess.h"

MediaProcessValidate::MediaProcessValidate(Media* media)
    : MediaProcess(media) {}

MediaProcessValidate::~MediaProcessValidate() { MediaProcess::~MediaProcess(); }

void MediaProcessValidate::parse(std::string data) {
  // Get the converted frame amount and fps
  if (RegexUtils::isMatch(data, "corrupt", std::regex::icase)) {
    this->media->setActivity(Activity::FAILED_CORRUPT);
  }

  else {
    std::string quality =
        RegexUtils::getFirstMatch(data, "q=(\\d+\\.\\d+|-\\d+\\.\\d+)");
    std::string completedFrames =
        RegexUtils::getFirstMatch(data, "frame=.+?(\\d+)");
    std::string fps = RegexUtils::getFirstMatch(data, "fps=(\\d+\\.\\d+|\\d+)");
    ;

    if (quality != "-1.0") this->media->working->quality = std::stof(quality);
    this->media->working->bitrate = -1;
    this->media->working->completedFrames = std::stoll(completedFrames);
    this->media->working->fps = std::stof(fps);
  }
}