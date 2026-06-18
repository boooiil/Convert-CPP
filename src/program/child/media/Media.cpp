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

#include "MediaProcessConversion.h"
#include "MediaProcessStatistics.h"
#include "MediaProcessValidate.h"
#include "src/program/Program.h"
#include "src/program/child/media/file/FileContainer.h"
#include "src/program/definitions/DefinitionRegistry.h"
#include "src/program/ffmpeg/FFmpegArgumentBuilder.h"
#include "src/program/settings/enums/Activity_N.h"
#include "src/program/settings/enums/Command_N.h"
#include "src/program/settings/enums/Encoders_N.h"
#include "src/program/settings/enums/HWAccelerators_N.h"
#include "src/utils/ListUtils.h"
#include "src/utils/NumberUtils.h"
#include "src/utils/StringUtils.h"
#include "src/utils/TimeUtils.h"
#include "src/utils/logging/LogColor.h"
#include "src/utils/logging/Logger.h"

Media::Media(Arguments &arguments, uuids::uuid id, std::string name,
             std::filesystem::path path)
    : id(id), started(0), ended(0), probeResult(nullptr),
      ffmpegArguments(nullptr), activity(Activity_N::WAITING),
      file(FileContainer(
          name, path,
          arguments.argumentRegistry.get<Command_N::QUALITY>().get().name,
          Container_N::definition(
              arguments.argumentRegistry.get<Command_N::CONTAINER>()))),
      arguments(arguments) {}

Media::~Media() {
  LOG_DEBUG(Logger::Priority::INFO, "Deconstructing media: ", this->id);

  if (probeResult != nullptr)
    delete probeResult;

  LOG_DEBUG(Logger::Priority::INFO, "Deconstructing ffmpegArguments: {}",
            static_cast<void *>(ffmpegArguments));
  if (ffmpegArguments != nullptr)
    delete ffmpegArguments;
}

Activity_N::Activity Media::getActivity() { return this->activity; }

const bool Media::isProcessing() const {
  return Activity_N::isProcessing(this->activity);
}

const bool Media::hasFailed() const {
  return Activity_N::isFailed(this->activity);
}

const bool Media::hasFinished() const {
  return this->activity == Activity_N::FINISHED;
}

const bool Media::isWaiting() const {
  return this->activity == Activity_N::WAITING;
}
const bool Media::isWaitingToStatistics() const {
  return this->activity == Activity_N::WAITING_STATISTICS;
}

const bool Media::isWaitingToConvert() const {
  return this->activity == Activity_N::WAITING_CONVERT;
}

const bool Media::isWaitingToValidate() const {
  return this->activity == Activity_N::WAITING_VALIDATE;
}

void Media::setActivity(Activity_N::Activity provided_activity) {
  this->activity = provided_activity;
}

void Media::doStatistics() {
  if (Program::stopFlag == true) {
    LOG_DEBUG(Logger::Priority::INFO, "Stopping statistics due to stop flag.");
    this->setActivity(Activity_N::FINISHED);
    return;
  }

  this->setActivity(Activity_N::STATISTICS);

  LOG_DEBUG(Logger::Priority::INFO,
            "Starting statistics for: ", this->file.naming.original_name_ext);

  MediaProcessStatistics statistics(this);
  statistics.start("ffprobe -v quiet -print_format json -show_format "
                   "-show_streams \"" +
                   this->file.naming.original_full_path.string() + "\"");

  if (this->hasFailed()) {
    return;
  }

  this->setActivity(Activity_N::WAITING_CONVERT);
}
void Media::doConversion() {
  if (Program::stopFlag == true) {
    LOG_DEBUG(Logger::Priority::INFO, "Stopping conversion due to stop flag.");
    this->setActivity(Activity_N::FAILED_SYSTEM);
    return;
  }

  this->setActivity(Activity_N::CONVERT);

  LOG_DEBUG(Logger::Priority::INFO,
            "Starting conversion for: ", this->file.naming.original_name_ext);

  MediaProcessConversion conversion(this);

  conversion.start("ffmpeg " +
                   ListUtils::join(this->ffmpegArguments->build(), " "));

  if (this->hasFailed()) {
    return;
  }

  if (!std::filesystem::exists(this->file.naming.conversion_full_path)) {
    LOG_DEBUG(Logger::Priority::ERROR,
              "Converted file does not exist: " +
                  this->file.naming.conversion_full_path.string());
    this->setActivity(Activity_N::FAILED_FILE_MISSING);
    return;
  } else {
    this->file.video_info.new_size =
        std::filesystem::file_size(this->file.naming.conversion_full_path);
  }

  this->setActivity(Activity_N::WAITING_VALIDATE);
}
void Media::doValidation() {
  if (Program::stopFlag == true) {
    LOG_DEBUG(Logger::Priority::INFO, "Stopping validation due to stop flag.");
    this->setActivity(Activity_N::FAILED_SYSTEM);
    return;
  }

  this->setActivity(Activity_N::VALIDATE);

  LOG_DEBUG(Logger::Priority::INFO,
            "Starting validation for: ", this->file.naming.original_name_ext);

  MediaProcessValidate validate(this);
  validate.start("ffmpeg -v quiet -stats -i \"" +
                 this->file.naming.conversion_full_path.string() +
                 "\" -f null -");
  if (this->hasFailed()) {
    return;
  }

  this->setActivity(Activity_N::FINISHED);
}

void Media::buildFFmpegArguments(bool isValidate) {
  this->ffmpegArguments = new FFmpegArgumentBuilder(this);
  this->ffmpegArguments->validate();
}

int Media::eta() const {
  float mediaFPS =
      this->file.processing_info.fps > 0 ? this->file.processing_info.fps : 1;
  auto totalFrames = this->file.video_info.totalFrames;
  auto completedFrames = this->file.processing_info.completedFrames;

  return static_cast<int>(ceil((totalFrames - completedFrames) / mediaFPS) *
                          1000);
}

int Media::percentCompleted() const {
  auto totalFrames = static_cast<double>(this->file.video_info.totalFrames);
  auto completedFrames = this->file.processing_info.completedFrames;

  return static_cast<int>(std::round((completedFrames / totalFrames) * 100));
}

float Media::quality() const {
  float crf = this->file.processing_info.quality;
  int v_crf = this->file.video_info.crf;

  return (v_crf / crf) * 100;
}

float Media::speed() const {
  float workingFPS = this->file.processing_info.fps;
  float videoFPS = this->file.video_info.fps;

  return workingFPS / videoFPS;
}

int Media::percentReduced() const {
  auto currSize = static_cast<double>(this->file.video_info.size);
  auto newSize = this->file.video_info.new_size;

  return static_cast<int>(std::round(((currSize - newSize) / currSize) * 100));
}

std::string Media::convertingLine() const {
  std::string accel_letter =
      HWAccelerators_N::getLetter(this->arguments.running_hw_accel);

  std::string encoder_letter =
      Encoders_N::getLetter(this->arguments.running_encoder);

  std::string activity_letter = Activity_N::getLetter(this->activity);

  std::string prefix = StringUtils::i_bracket(
      activity_letter + accel_letter + encoder_letter, "");

  // create a time util to get this
  std::string started =
      StringUtils::bracket("STR", TimeUtils::timeFormat(this->started));

  // create a time util to get this
  std::string eta =
      StringUtils::bracket("ETA", TimeUtils::durationFormat(this->eta()));

  std::string fileName = StringUtils::bracket(
      "FILE",
      StringUtils::truncateString(this->file.naming.conversion_name, 25));

  std::string activity = StringUtils::bracket(
      "ACT", DefinitionRegistry::defFromEnum(this->activity));

  std::string progress = StringUtils::bracket(
      "PROG", std::to_string(this->percentCompleted()) + "%");

  std::string cq = StringUtils::bracket(
      "QUAL", NumberUtils::formatNumber(this->quality(), 2) + "%");

  std::string speed = StringUtils::bracket(
      "SPEED", NumberUtils::formatNumber(this->speed(), 2));

  std::string bitrate = StringUtils::bracket(
      "BITRATE",
      NumberUtils::formatNumber(this->file.processing_info.bitrate, 2) +
          "kb/s");

  return prefix + " " + fileName + " " + started + " " + progress + " " + cq +
         " " + bitrate + " " + speed + " " + eta + "\n";
}

std::string Media::pendingLine() const {
  std::string fileName = StringUtils::bracket(
      "FILE",
      StringUtils::truncateString(this->file.naming.conversion_name, 25));

  std::string activity = StringUtils::bracket(
      "ACT", DefinitionRegistry::defFromEnum(this->activity));

  if (this->hasFinished()) {

    std::string ended =
        StringUtils::bracket("END", TimeUtils::timeFormat(this->ended));

    std::string elapsed = StringUtils::bracket(
        "ELAPSED",
        TimeUtils::durationFormat((this->ended - this->started) * 1000));

    std::string reduced = StringUtils::bracket(
        "REDUCED", std::to_string(this->percentReduced()) + "%");

    return fileName + " " + activity + " " + reduced + " " + ended + " " +
           elapsed + "\n";

  } else {
    return fileName + " " + activity + "\n";
  }
}

void Media::fromJSON(const nlohmann::json &json) {
  if (json.empty()) {
    LOG_DEBUG(Logger::Priority::INFO, "JSON is empty.");
    return;
  }

  nlohmann::json json_file = json["file"];
  nlohmann::json json_video = json["video"];
  nlohmann::json json_working = json["working"];

  std::string j_activity = json["activity"];

  this->activity =
      DefinitionRegistry::enumFromDefName<Activity_N::Activity>(j_activity);
  this->ended = json["ended"];
  this->started = json["started"];
  // this->ffmpegArguments = json["ffmpegArguments"];

  this->file.naming.original_name_ext = json_file["originalFileNameExt"];
  this->file.naming.original_full_path =
      std::filesystem::path(std::string(json_file["originalFullPath"]));
  this->file.naming.conversion_full_path =
      std::filesystem::path(std::string(json_file["conversionFilePath"]));
  this->file.naming.conversion_folder_path =
      std::filesystem::path(std::string(json_file["conversionFolderPath"]));
  this->file.naming.conversion_name = json_file["conversionName"];
  this->file.naming.conversion_name_ext = json_file["conversionNameExt"];
  this->file.naming.extension = json_file["ext"];
  this->file.video_info.size = json_file["size"];
  this->file.video_info.new_size = json_file["newSize"];
  this->file.processing_info.quality = json_file["quality"];
  this->file.naming.series = json_file["series"];
  this->file.naming.season_no = json_file["season"];

  // this->arguments.CWD = json_file["cwd"];

  this->file.video_info.fps = json_video["fps"];
  this->file.video_info.totalFrames = json_video["totalFrames"];
  this->file.video_info.subtitleProvider = json_video["subtitleProvider"];
  this->file.video_info.width = json_video["width"];
  this->file.video_info.height = json_video["height"];
  this->file.video_info.ratio = json_video["ratio"];
  this->file.video_info.convertedResolution = json_video["convertedResolution"];
  this->file.video_info.convertedHeight = json_video["convertedHeight"];
  this->file.video_info.convertedWidth = json_video["convertedWidth"];
  this->file.video_info.crop = json_video["crop"];
  this->file.video_info.crf = json_video["crf"];

  this->file.processing_info.fps = json_working["fps"];
  this->file.processing_info.completedFrames = json_working["completedFrames"];
  this->file.processing_info.quality = json_working["quality"];
  this->file.processing_info.bitrate = json_working["bitrate"];
}

nlohmann::json Media::toJSON(void) {
  nlohmann::json json;

  json["file"] = this->file.toJSON();

  if (this->ffmpegArguments == nullptr) {
    LOG_DEBUG(Logger::Priority::WARNING, "Media ffmpegArguments is null:",
              this->file.naming.original_name_ext);
  } else {
    // this might not work
    json["ffmpegArguments"] = this->ffmpegArguments->build();
  }

  json["activity"] = Activity_N::definition(this->getActivity());
  json["started"] = this->started;
  json["ended"] = this->ended;

  return json;
}
