/*********************************************************************
 * @file   Media.cpp
 * @brief  Media class implementation file.
 *
 * @author boooiil
 * @date   January 2024
 *********************************************************************/

#include "Media.h"

#include <filesystem>
#include <nlohmann/json.hpp>
#include <string>
#include <vector>

#include "../../../utils/ListUtils.h"
#include "../../../utils/StringUtils.h"
#include "../../../utils/logging/Logger.h"
#include "../../Program.h"
#include "../../settings/Settings.h"
#include "../../settings/arguments/ArgumentRegistry.h"
#include "../../settings/arguments/FlagArgument.h"
#include "../../settings/arguments/StringArgument.h"
#include "../../settings/arguments/TimeStringVectorArgument.h"
#include "../../settings/arguments/VectorArgument.h"
#include "../../settings/arguments/video/Quality.h"
#include "../../settings/enums/Activity.h"
#include "../../settings/enums/HWAccelerators.h"
#include "../../settings/enums/StringEnumDataHolder.h"
#include "../../settings/enums/Tunes.h"
#include "MediaFile.h"
#include "MediaFormat.h"
#include "MediaProcessConversion.h"
#include "MediaProcessStatistics.h"
#include "MediaProcessValidate.h"
#include "MediaVideoProperties.h"
#include "MediaWorkingProperties.h"

Media::Media()
    : started(0),
      ended(0),
      activity(Activity::WAITING),
      file(new MediaFile()),
      probeResult(nullptr),
      video(new MediaVideoProperties()),
      working(new MediaWorkingProperties()) {
  std::random_device rd;   // Seed for random number generator
  std::mt19937 gen(rd());  // Standard mersenne_twister_engine seeded with rd()
  uuids::uuid_random_generator generator(
      gen);  // Pass the generator to uuid_random_generator
  this->id = generator();
}

Media::Media(uuids::uuid id, std::string name, std::string path)
    : id(id),
      started(0),
      ended(0),
      activity(Activity::WAITING),
      file(new MediaFile(id, name, path)),
      probeResult(nullptr),
      video(new MediaVideoProperties()),
      working(new MediaWorkingProperties()) {}

Media::~Media() {
  LOG_DEBUG("Deconstructing media: ", this->file->originalFileNameExt);

  if (file != nullptr) delete file;
  if (video != nullptr) delete video;
  if (working != nullptr) delete working;
  if (probeResult != nullptr) delete probeResult;
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
  this->setActivity(Activity::STATISTICS);

  LOG_DEBUG("Starting statistics for: ", this->file->originalFileNameExt);

  MediaProcessStatistics statistics(this);
  statistics.start(
      "ffprobe -v quiet -print_format json -show_format "
      "-show_streams \"" +
      this->file->originalFullPath + "\"");

  if (this->hasFailed()) {
    return;
  }

  this->setActivity(Activity::WAITING_CONVERT);
}
void Media::doConversion() {
  this->setActivity(Activity::CONVERT);

  LOG_DEBUG("Starting conversion for: ", this->file->originalFileNameExt);

  MediaProcessConversion conversion(this);

  conversion.start("ffmpeg " + ListUtils::join(Media::ffmpegArguments, " "));

  if (this->hasFailed()) {
    return;
  }

  this->file->newSize =
      std::filesystem::file_size(this->file->conversionFilePath);

  this->setActivity(Activity::WAITING_VALIDATE);
}
void Media::doValidation() {
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
  ProgramOptions& programSettings = *Program::settings->programOptions;
  ChildOptions& childOptions = *Program::settings->childOptionsMap[id];
  ArgumentRegistry& argumentRegistry = *childOptions.argumentRegistry;
  MediaFormat format = argumentRegistry.get_t<Quality>(Command::QUALITY)->get();

  this->ffmpegArguments.clear();

  this->ffmpegArguments.push_back("-v error -stats");

  if (argumentRegistry.get_t<FlagArgument>(Command::HARDWAREDECODE)->get()) {
    if (childOptions.runningHWAccel != HWAccelerators::INVALID) {
      this->ffmpegArguments.push_back(
          "-hwaccel " +
          EnumToStringFactory::get<HWAccelerators>(childOptions.runningHWAccel)
              .getName());
    }
  }

  this->ffmpegArguments.push_back("-i \"" + this->file->originalFullPath +
                                  "\"");

  this->ffmpegArguments.push_back("-map 0:v:0");

  VectorArgument<int>* audioStreams =
      argumentRegistry.get_t<VectorArgument<int>>("-as");

  if (!audioStreams->get().empty()) {
    // TODO: make vector argument iterable
    for (const int stream : audioStreams->get()) {
      this->ffmpegArguments.push_back("-map 0:a:" + std::to_string(stream));
    }
  } else {
    this->ffmpegArguments.push_back("-map 0:a?");
  }

  this->ffmpegArguments.push_back("-map 0:s?");
  this->ffmpegArguments.push_back("-map 0:t?");

  this->ffmpegArguments.push_back("-c:t copy");

  /***************************************************
   *                                                 *
   *                  AUDIO SETTINGS                 *
   *                                                 *
   ***************************************************/

  // cases:
  // 1. audio streams match audio formats
  // 2. audio streams are greater than audio formats
  // 3. audio streams are less than audio formats
  // 4. audio streams are empty

  // TODO:
  // if audio streams used, iterate only over those audio streams
  // else iterate over all audio streams

  for (int i = 0; i < this->probeResult->audioStreams.size(); i++) {
    // if audio streams exist
    if (!audioStreams->get().empty()) {
      // and if the audio stream is not in the list
      // skip the audio stream
      if (!ListUtils::contains(audioStreams->get(), i)) {
        LOG_DEBUG("Skipping audio stream", i, "for not being in the list.");
        continue;
      }
    }

    LOG_DEBUG("Audio stream", i, "is in the list.");

    bool afCopy = false;
    int usingChannels = this->probeResult->audioStreams[i].channels;

    std::string usingFormat = this->probeResult->audioStreams[i].codec_name;

    std::string codecMap = "-c:a:" + std::to_string(i);
    std::string metadataMap = "-metadata:s:a:" + std::to_string(i);
    std::string channelMap = "-ac:a:" + std::to_string(i);

    std::vector<std::string> audioFormats =
        argumentRegistry
            .get_t<VectorArgument<std::string>>(Command::AUDIOCODEC)
            ->get();
    std::vector<int> audioChannels =
        argumentRegistry.get_t<VectorArgument<int>>(Command::AUDIOCHANNELS)
            ->get();

    // if there is an audio format within the list
    // at index use the format
    if (audioFormats.size() > i) {
      LOG_DEBUG("Audio index [", i, "] is using codec (", audioFormats[i], ")");
      usingFormat = audioFormats[i];
      this->ffmpegArguments.push_back(codecMap + " " + usingFormat);

    }
    // if the formats are not empty
    // but are under the stream count
    // use the last format
    else if (!audioFormats.empty()) {
      LOG_DEBUG("Audio index [", i, "] exceeded codecs, using last codec (",
                audioFormats[audioFormats.size() - 1], ")");
      usingFormat = audioFormats[audioFormats.size() - 1];
      this->ffmpegArguments.push_back(codecMap + " " + usingFormat);
    }
    // else flag that the audio format is to be copied
    else {
      LOG_DEBUG("Audio index [", i, "] marked for copy codec.");
      afCopy = true;
    }

    // if there are provided audio channels
    if (!audioChannels.empty()) {
      LOG_DEBUG("Audio index [", i, "] has provided channels.");
      // if the audio formats are empty
      // use the existing audio codec
      if (afCopy) {
        LOG_DEBUG("Audio index [", i,
                  "] has channels but needs codec to proceed, using codec copy "
                  "with channels (",
                  audioChannels[i], ")");
        std::string codec = this->probeResult->audioStreams[i].codec_name;
        this->ffmpegArguments.push_back(codecMap + " " + codec);
      }

      // if the audio channels exceed the audio streams
      // use the channel at the stream index
      if (audioChannels.size() > i) {
        LOG_DEBUG("Audio index [", i, "] is using channel (", audioChannels[i],
                  ")");
        usingChannels = audioChannels[i];

        this->ffmpegArguments.push_back(channelMap + " " +
                                        std::to_string(usingChannels));
      }
      // else use the last audio channel
      else {
        LOG_DEBUG("Audio index [", i,
                  "] exceeded channels, using last channel (",
                  audioChannels[audioChannels.size() - 1], ")");
        usingChannels = audioChannels[audioChannels.size() - 1];

        this->ffmpegArguments.push_back(channelMap + " " +
                                        std::to_string(usingChannels));
      }
    } else {
      // if the audio channels are empty
      // and the audio formats are empty
      // copy the existing audio stream
      if (afCopy) {
        LOG_DEBUG("Audio index [", i,
                  "] had no defined audio codec or channels, setting copy.");
        this->ffmpegArguments.push_back(codecMap + " copy");
      }
      // if the audio channels are empty
      // and the audio formats are not empty
      // use the default auido channels
      else {
        std::string channels =
            std::to_string(this->probeResult->audioStreams[i].channels);
        LOG_DEBUG("Audio index [", i,
                  "] had audio codec but no channels, using default channels (",
                  channels, ")");
        ;
        this->ffmpegArguments.push_back(channelMap + " " + channels);
      }
    }

    std::string channelType;

    switch (usingChannels) {
      case 1:
        channelType = "Mono";
        break;
      case 2:
        channelType = "Stereo";
        break;
      case 6:
        channelType = "5.1";
        break;
      case 8:
        channelType = "7.1";
        break;
      default:
        channelType = std::to_string(usingChannels);
    }

    LOG_DEBUG("Audio index [", i, "] was set a title of (", usingFormat,
              channelType, ").");

    usingFormat = StringUtils::toUpperCase(usingFormat);

    this->ffmpegArguments.push_back(metadataMap + " title=\"" + usingFormat +
                                    " " + channelType + "\"");
  }

  this->ffmpegArguments.push_back(
      "-c:v " + EnumToStringFactory::get<Encoders>(childOptions.runningEncoder)
                    .getName());

  this->ffmpegArguments.push_back("-preset slow");

  this->ffmpegArguments.push_back("-level 4.1");

  if (argumentRegistry.get_t<FlagArgument>(Command::BITRATE)->get()) {
    this->ffmpegArguments.push_back("-b:v " + std::to_string(format.bitrate) +
                                    "M");
    this->ffmpegArguments.push_back("-bufsize " +
                                    std::to_string(format.bitrate * 2) + "M");
    this->ffmpegArguments.push_back("-maxrate " +
                                    std::to_string(format.max * 2) + "M");
    this->ffmpegArguments.push_back("-minrate " +
                                    std::to_string(format.min * 2) + "M");
  } else if (argumentRegistry.get_t<FlagArgument>(Command::CONSTRAIN)->get()) {
    this->ffmpegArguments.push_back("-crf " + std::to_string(format.crf));
    this->ffmpegArguments.push_back("-bufsize " +
                                    std::to_string(format.bitrate * 2) + "M");
    this->ffmpegArguments.push_back("-maxrate " +
                                    std::to_string(format.max * 2) + "M");
  } else {
    this->ffmpegArguments.push_back("-crf " + std::to_string(format.crf));
  }

  if (argumentRegistry.get_t<FlagArgument>(Command::CROP)->get()) {
    this->ffmpegArguments.push_back(
        "-vf scale=" + this->video->convertedResolution +
        ":flags=lanczos,crop=" + format.crop);

  }

  else
    this->ffmpegArguments.push_back(
        "-vf scale=" + this->video->convertedResolution + ":flags=lanczos");

  StringArgument* startBeginning =
      argumentRegistry.get_t<StringArgument>("-ss");

  if (!startBeginning->get().empty()) {
    this->ffmpegArguments.push_back("-ss " + startBeginning->get());
  }

  TimeStringVectorArgument* trim =
      argumentRegistry.get_t<TimeStringVectorArgument>("-tr");

  if (!trim->get().empty()) {
    this->ffmpegArguments.push_back("-ss " + trim->get()[0]);
    this->ffmpegArguments.push_back("-to " + trim->get()[1]);
  }

  /** TODO: flesh out later */
  // if (this.video.subtitle_provider) {

  //     if (this.video.subtitle_provider === "mov")
  //     this.ffmpeg_argument.push("-c:s mov_text") else
  //     this.ffmpeg_argument.push("-c:s copy")

  // }

  this->ffmpegArguments.push_back("-c:s copy");

  if (argumentRegistry.get_t<BaseArgument<Tunes>>(Command::TUNE)->get() !=
      Tunes::DEFAULT) {
    this->ffmpegArguments.push_back(
        "-tune " +
        EnumToStringFactory::get<Tunes>(
            argumentRegistry.get_t<BaseArgument<Tunes>>(Command::TUNE)->get())
            .getName());
  }

  this->ffmpegArguments.push_back("\"" + this->file->conversionFilePath + "\"");

  if (isValidate ||
      argumentRegistry.get_t<FlagArgument>(Command::OVERWRITE)->get())
    this->ffmpegArguments.push_back("-y");
  else {
    this->ffmpegArguments.push_back("-n");
  }

  LOG_DEBUG("FFMPEG ARGUMENTS: ", ListUtils::join(this->ffmpegArguments, " "));
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
  this->ffmpegArguments = json["ffmpegArguments"];

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