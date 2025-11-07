/*********************************************************************
 * @file   Media.cpp
 * @brief  Media class implementation file.
 *
 * @author boooiil
 * @date   January 2024
 *********************************************************************/

#include "Media.h"

#include <nlohmann/json.hpp>
#include <string>

#include "../../../utils/ListUtils.h"
#include "../../../utils/logging/Logger.h"
#include "../../settings/enums/Activity.h"
#include "../ffmpeg/FFmpegArgumentBuilder.h"
#include "MediaFile.h"
#include "MediaProcessConversion.h"
#include "MediaProcessStatistics.h"
#include "MediaProcessValidate.h"
#include "MediaVideoProperties.h"
#include "MediaWorkingProperties.h"
#include "src/program/Program.h"
#include "src/program/settings/enums/EnumToStringFactory.h"

Media::Media()
    : started(0), ended(0), probeResult(nullptr), file(new MediaFile()),
      video(new MediaVideoProperties()), working(new MediaWorkingProperties()),
      ffmpegArguments(nullptr), activity(Activity::WAITING) {
  std::random_device rd;  // Seed for random number generator
  std::mt19937 gen(rd()); // Standard mersenne_twister_engine seeded with rd()
  uuids::uuid_random_generator generator(
      gen); // Pass the generator to uuid_random_generator
  this->id = generator();
}

Media::Media(uuids::uuid id, std::string name, std::string path)
    : id(id), started(0), ended(0), probeResult(nullptr),
      file(new MediaFile(id, name, path)), video(new MediaVideoProperties()),
      working(new MediaWorkingProperties()), ffmpegArguments(nullptr),
      activity(Activity::WAITING) {}

Media::~Media() {
  LOG_DEBUG("Deconstructing media: ", this->id);

  if (file != nullptr)
    delete file;
  if (video != nullptr)
    delete video;
  if (working != nullptr)
    delete working;
  if (probeResult != nullptr)
    delete probeResult;

  LOG_DEBUG("Deleting ffmpegArguments: {}",
            static_cast<void *>(ffmpegArguments));
  if (ffmpegArguments != nullptr)
    delete ffmpegArguments;
}

Activity Media::getActivity() { return Media::activity; }

const bool Media::isProcessing() {
  return Media::activity == Activity::STATISTICS ||
         Media::activity == Activity::CONVERT ||
         Media::activity == Activity::VALIDATE;
}

const bool Media::hasFailed() {
  switch (Media::activity) {
  case Activity::FAILED:
  case Activity::FAILED_CODEC:
  case Activity::FAILED_CONTAINER:
  case Activity::FAILED_CORRUPT:
  case Activity::FAILED_FILE:
  case Activity::FAILED_FILE_MISSING:
  case Activity::FAILED_FILE_NOT_RECOGNIZED:
  case Activity::FAILED_FILE_PERMISSIONS:
  case Activity::FAILED_HARDWARE:
  case Activity::FAILED_INVALID_AUDIO_CHANNELS:
  case Activity::FAILED_INVALID_AUDIO_STREAMS:
  case Activity::FAILED_INVALID_DURATION_SS:
  case Activity::FAILED_INVALID_DURATION_TO:
  case Activity::FAILED_INVALID_ENCODER:
  case Activity::FAILED_JSON_PARSE:
  case Activity::FAILED_SYSTEM:
    return true;
    break;
  default:
    return false;
    break;
  }
}

const bool Media::hasFinished() {
  return Media::activity == Activity::FINISHED;
}

const bool Media::isWaiting() { return Media::activity == Activity::WAITING; }

const bool Media::isWaitingToStatistics() {
  return Media::activity == Activity::WAITING_STATISTICS;
}

const bool Media::isWaitingToConvert() {
  return Media::activity == Activity::WAITING_CONVERT;
}

const bool Media::isWaitingToValidate() {
  return Media::activity == Activity::WAITING_VALIDATE;
}

void Media::setActivity(Activity provided_activity) {
  Media::activity = provided_activity;
}

void Media::doStatistics() {
  if (Program::stopFlag == true) {
    LOG_DEBUG("Stopping statistics due to stop flag.");
    this->setActivity(Activity::FINISHED);
    return;
  }

  this->setActivity(Activity::STATISTICS);

  LOG_DEBUG("Starting statistics for: ", this->file->originalFileNameExt);

  MediaProcessStatistics statistics(this);
  statistics.start("ffprobe -v quiet -print_format json -show_format "
                   "-show_streams \"" +
                   this->file->originalFullPath + "\"");

  if (this->hasFailed()) {
    return;
  }

  this->setActivity(Activity::WAITING_CONVERT);
}
void Media::doConversion() {
  if (Program::stopFlag == true) {
    LOG_DEBUG("Stopping conversion due to stop flag.");
    this->setActivity(Activity::FAILED_SYSTEM);
    return;
  }

  this->setActivity(Activity::CONVERT);

  LOG_DEBUG("Starting conversion for: ", this->file->originalFileNameExt);

  MediaProcessConversion conversion(this);

  conversion.start("ffmpeg " +
                   ListUtils::join(Media::ffmpegArguments->build(), " "));

  if (this->hasFailed()) {
    return;
  }

  if (!std::filesystem::exists(this->file->conversionFilePath)) {
    LOG_DEBUG("Converted file does not exist: ",
              this->file->conversionFilePath);
    this->setActivity(Activity::FAILED_FILE_MISSING);
    return;
  } else {
    this->file->newSize =
        std::filesystem::file_size(this->file->conversionFilePath);
  }

  this->setActivity(Activity::WAITING_VALIDATE);
}
void Media::doValidation() {

  if (Program::stopFlag == true) {
    LOG_DEBUG("Stopping validation due to stop flag.");
    this->setActivity(Activity::FAILED_SYSTEM);
    return;
  }

  this->setActivity(Activity::VALIDATE);

  LOG_DEBUG("Starting validation for: ", this->file->originalFileNameExt);

  MediaProcessValidate validate(this);
  validate.start("ffmpeg -v quiet -stats -i \"" +
                 this->file->conversionFilePath + "\" -f null -");

  if (this->hasFailed()) {
    return;
  }

  this->setActivity(Activity::FINISHED);
}

void Media::buildFFmpegArguments(bool isValidate) {
  this->ffmpegArguments = new FFmpegArgumentBuilder(this);
  this->ffmpegArguments->validate();
}

int Media::eta() const {

  float mediaFPS = this->working->fps > 0 ? this->working->fps : 1;
  auto totalFrames = this->video->totalFrames;
  auto completedFrames = this->working->completedFrames;

  return static_cast<int>(ceil((totalFrames - completedFrames) / mediaFPS) *
                          1000);
}

int Media::percentCompleted() const {

  auto totalFrames = static_cast<double>(this->video->totalFrames);
  auto completedFrames = this->working->completedFrames;

  return static_cast<int>(std::round((completedFrames / totalFrames) * 100));
}

float Media::quality() const {
  float crf = this->working->quality;
  int v_crf = this->video->crf;

  return (v_crf / crf) * 100;
}

float Media::speed() const {
  float workingFPS = this->working->fps;
  float videoFPS = this->video->fps;

  return workingFPS / videoFPS;
}

int Media::percentReduced() const {
  auto currSize = static_cast<double>(this->file->size);
  auto newSize = this->file->newSize;

  return static_cast<int>(std::round(((currSize - newSize) / currSize) * 100));
}

void Media::fromJSON(nlohmann::json json) {
  if (json.empty()) {
    LOG_DEBUG("JSON is empty.");
    return;
  }

  nlohmann::json json_file = json["file"];
  nlohmann::json json_video = json["video"];
  nlohmann::json json_working = json["working"];

  std::string j_activity = json["activity"];

  this->activity = EnumToStringFactory::fromName<Activity>(j_activity);
  this->ended = json["ended"];
  this->started = json["started"];
  // this->ffmpegArguments = json["ffmpegArguments"];

  this->file->originalFileNameExt = json_file["originalFileNameExt"];
  this->file->originalFullPath = json_file["originalFullPath"];
  this->file->conversionFilePath = json_file["conversionFilePath"];
  this->file->conversionFolderPath = json_file["conversionFolderPath"];
  this->file->conversionName = json_file["conversionName"];
  this->file->conversionNameExt = json_file["conversionNameExt"];
  this->file->ext = json_file["ext"];
  this->file->size = json_file["size"];
  this->file->newSize = json_file["newSize"];
  this->file->cwd = json_file["cwd"];
  this->file->quality = json_file["quality"];
  this->file->series = json_file["series"];
  this->file->season = json_file["season"];

  this->video->fps = json_video["fps"];
  this->video->totalFrames = json_video["totalFrames"];
  this->video->subtitleProvider = json_video["subtitleProvider"];
  this->video->width = json_video["width"];
  this->video->height = json_video["height"];
  this->video->ratio = json_video["ratio"];
  this->video->convertedResolution = json_video["convertedResolution"];
  this->video->convertedHeight = json_video["convertedHeight"];
  this->video->convertedWidth = json_video["convertedWidth"];
  this->video->crop = json_video["crop"];
  this->video->crf = json_video["crf"];

  this->working->fps = json_working["fps"];
  this->working->completedFrames = json_working["completedFrames"];
  this->working->quality = json_working["quality"];
  this->working->bitrate = json_working["bitrate"];
}

nlohmann::json Media::toJSON(void) {
  nlohmann::json json;

  return json;
}
