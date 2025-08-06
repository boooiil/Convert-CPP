#include "Child.h"

#include <filesystem>
#include <nlohmann/json.hpp>
#include <queue>
#include <string>
#include <thread>
#include <vector>

#include "../../utils/DirectoryUtils.h"
#include "../../utils/TimeUtils.h"
#include "../../utils/logging/LogColor.h"
#include "../../utils/logging/Logger.h"
#include "../Program.h"
#include "../settings/Settings.h"
#include "../settings/arguments/ArgumentRegistry.h"
#include "../settings/arguments/FlagArgument.h"
#include "../settings/arguments/IntegerArgument.h"
#include "../settings/enums/Activity.h"
#include "../settings/enums/EnumToStringFactory.h"
#include "media/Media.h"


std::vector<std::thread> workerThreads;

// template <typename T>
// typename ArgumentRegistry::getTFn<T> get_t =
// Child::settings->argumentRegistry.get<T>;

Child::Child(void) : endable(false) {}

void Child::prepare(std::vector<std::string> &args) {
  // initialize settings

  std::random_device rd;  // Seed for random number generator
  std::mt19937 gen(rd()); // Standard mersenne_twister_engine seeded with rd()
  uuids::uuid_random_generator generator(
      gen); // Pass the generator to uuid_random_generator
  this->id = generator();

  Program::settings->childOptionsMap[this->id] = new ChildOptions();
  ChildOptions &childOptions = *Program::settings->childOptionsMap[this->id];

  childOptions.prepare();
  childOptions.parse(args);
  childOptions.validate();

  std::vector<std::filesystem::directory_entry> files =
      DirectoryUtils::getFilesInDirectory(args[0],
                                          std::vector{".mp4", ".mkv", ".avi"});

  for (std::filesystem::directory_entry file : files) {
    std::string cwd = file.path().parent_path().string();
    std::string filename = file.path().filename().string();

    Media *media = new Media(this->id, filename, cwd);
    media->file->rename();

    if (!std::filesystem::exists(media->file->conversionFolderPath) &&
        !childOptions.argumentRegistry->get_t<FlagArgument>(Command::INFO)
             ->get()) {
      LOG_DEBUG("Creating directory: ", media->file->conversionFolderPath);
      std::filesystem::create_directory(media->file->conversionFolderPath);
    }

    this->pending.push(media);
  }
}

// void Child::prepare(ProcessSettings* settings) {
//   this->processSettings = settings;
//
//   std::vector<std::filesystem::directory_entry> files =
//     DirectoryUtils::getFilesInDirectory(this->processSettings->cwd,
//     std::vector{ ".mkv", ".avi" });
//
//   for (std::filesystem::directory_entry file : files) {
//     std::string cwd = file.path().parent_path().string();
//     std::string filename = file.path().filename().string();
//
//     Media* media = new Media(filename, cwd);
//     media->file->rename(this->processSettings);
//
//     if (!std::filesystem::exists(media->file->conversionFolderPath) &&
//       !this->processSettings->argumentParser->argumentRegistry.get_t<FlagArgument>("-i")->get())
//       { LOG_DEBUG("Creating directory: ", media->file->conversionFolderPath);
//       std::filesystem::create_directory(media->file->conversionFolderPath);
//     }
//
//     this->pending.push(media);
//   }
// }

void Child::run(void) {
  // once every second
  this->setEndable(false);
  int currentAmount = static_cast<int>(this->converting.size());

  ChildOptions &childOptions = *Program::settings->childOptionsMap[this->id];

  IntegerArgument *setAmount =
      childOptions.argumentRegistry->get_t<IntegerArgument>(Command::AMOUNT);

  LOG_DEBUG(std::to_string(currentAmount), setAmount->toString());

  if ((currentAmount < (int)*setAmount) && !this->pending.empty()) {
    Media *media = this->pending.front();

    // if there are no media files waiting
    // and the current amount of converting media is 0
    // then exit the program
    if (!media->isWaiting()) {
      LOG_DEBUG("Media is not waiting:", media->file->originalFileNameExt,
                EnumToStringFactory::get(media->getActivity()).getName());
      if (currentAmount == 0) {
        this->setEndable(true);
        Program::stopFlag = true;
      }
    } else {
      LOG_DEBUG("Queued media for encoding:", media->file->originalFileNameExt,
                EnumToStringFactory::get(media->getActivity()).getName());
      media->setActivity(Activity::WAITING_STATISTICS);

      media->started = TimeUtils::getEpoch();

      this->converting.push(media);
      this->pending.pop();
    }

    this->setEndable(true);
  }

  // error if there are more converting than allowed
  if (currentAmount > (int)*setAmount) {
    LOG(LogColor::fgRed(
        "CURRENT TRANSCODES ARE GREATER THAN THE ALLOWED AMOUNT."));

    LOG(LogColor::fgRed("CURRENT ALLOWED AMOUNT: " + setAmount->toString()));

    LOG(LogColor::fgRed("CURRENT QUEUE: " + setAmount->toString()));

    // iterate over converting
    while (!this->converting.empty()) {
      Media *value = this->converting.front();
      LOG(LogColor::fgRed("CURRENT FILE: " + value->file->conversionName));
      this->converting.pop();
    }

    this->setEndable(true);
  }

  // temp queue for conversion iteration
  std::queue<Media *> t_queue;

  // iterate over converting media
  while (!this->converting.empty()) {
    Media *media = this->converting.front();

    LOG_DEBUG(media->file->originalFileNameExt,
              EnumToStringFactory::get(media->getActivity()).getName());

    if (!media->isProcessing()) {
      LOG_DEBUG("Media is not processing:", media->file->originalFileNameExt,
                EnumToStringFactory::get(media->getActivity()).getName());

      if (media->isWaitingToStatistics()) {
        media->doStatistics();
      } else if (media->isWaitingToConvert()) {
        media->buildFFmpegArguments(false);
        workerThreads.emplace_back([media]() { media->doConversion(); });
      } else if (media->isWaitingToValidate()) {
        workerThreads.emplace_back([media]() { media->doValidation(); });
      }
    }

    if (media->hasFailed() || media->hasFinished()) {
      // todo: again, chrono stuff
      media->ended = TimeUtils::getEpoch();

      LOG_DEBUG("Media ended:", media->file->conversionName);
      this->pending.push(media);
      LOG_DEBUG("pending size after finish:",
                std::to_string(this->pending.size()));
    } else {
      t_queue.push(media);
    }
    this->converting.pop();
  }

  LOG_DEBUG("t_queue size:", std::to_string(t_queue.size()));
  this->converting = t_queue;

  // if (this->processSettings->argumentParser->loggingFormat.get() ==
  //   LoggingOptions::DEBUG) {
  //   // Ticker::display->printDebug();
  //   // should not need to print in this class
  // }
  // else if (LoggingOptions::isJSON(
  //   this->processSettings->argumentParser->loggingFormat)) {
  //   // Ticker::display->printJSON();
  //   // should not need to print in this class
  // }
  // else {
  //   // Ticker::display->print();
  //   // should not need to print in this class
  // }
}

void Child::end(void) {
  LOG_DEBUG("Ending child runner.");
  LOG_DEBUG("Expected to delete { pending[], converting[], settings }.");
  // iterate over running threads and join
  for (auto &t : workerThreads) {
    if (t.joinable()) {
      t.join();
    }
  }

  // call threads descructor and clear the vector
  workerThreads.clear();

  // iterate over pending
  while (!this->pending.empty()) {
    Media *media = this->pending.front();
    this->pending.pop();

    LOG_DEBUG("Deleting media in:", media->file->originalFileNameExt);

    delete media;
  }

  // iterate over converting
  while (!this->converting.empty()) {
    Media *media = this->converting.front();
    this->converting.pop();

    LOG_DEBUG("Deleting media in:", media->file->originalFileNameExt);

    delete media;
  }

  /*if (this->processSettings != nullptr) {
    LOG_DEBUG("Deleting settings.");
    delete this->processSettings;
  }*/
}

void Child::setEndable(bool flag) {
  LOG_DEBUG("Child has been set as endable:", this->endable ? "True" : "False");
  this->endable = flag;
}

bool Child::isEndable(void) { return this->endable; }

void Child::fromJSON(nlohmann::json) {}

nlohmann::json Child::toJSON() {
  using namespace nlohmann;

  json child;

  std::queue<Media *> t_queue;

  child["pending"] = nlohmann::json::array();
  child["converting"] = nlohmann::json::array();

  // converting file
  while (!this->converting.empty()) {
    Media *media = this->converting.front();

    nlohmann::json mediaDebug;
    nlohmann::json mediaFileDebug;
    nlohmann::json mediaVideoDebug;
    nlohmann::json mediaWorkingDebug;

    mediaDebug["activity"] =
        EnumToStringFactory::get(media->getActivity()).getName();
    mediaDebug["started"] = media->started;
    mediaDebug["ended"] = media->ended;
    // this might not work
    mediaDebug["ffmpegArguments"] = media->ffmpegArguments->build();

    mediaFileDebug["originalFileNameExt"] = media->file->originalFileNameExt;
    mediaFileDebug["originalFullPath"] = media->file->originalFullPath;
    mediaFileDebug["conversionName"] = media->file->conversionName;
    mediaFileDebug["conversionNameExt"] = media->file->conversionNameExt;
    mediaFileDebug["conversionFolderPath"] = media->file->conversionFolderPath;
    mediaFileDebug["conversionFilePath"] = media->file->conversionFilePath;
    mediaFileDebug["ext"] = media->file->ext;
    mediaFileDebug["size"] = media->file->size;
    mediaFileDebug["newSize"] = media->file->newSize;
    mediaFileDebug["cwd"] = media->file->cwd;
    mediaFileDebug["quality"] = media->file->quality;
    mediaFileDebug["series"] = media->file->series;
    mediaFileDebug["season"] = media->file->season;

    mediaVideoDebug["fps"] = media->video->fps;
    mediaVideoDebug["totalFrames"] = media->video->totalFrames;
    mediaVideoDebug["subtitleProvider"] = media->video->subtitleProvider;
    mediaVideoDebug["width"] = media->video->width;
    mediaVideoDebug["height"] = media->video->height;
    mediaVideoDebug["ratio"] = media->video->ratio;
    mediaVideoDebug["convertedWidth"] = media->video->convertedWidth;
    mediaVideoDebug["convertedHeight"] = media->video->convertedHeight;
    mediaVideoDebug["convertedResolution"] = media->video->convertedResolution;
    mediaVideoDebug["crop"] = media->video->crop;
    mediaVideoDebug["crf"] = media->video->crf;

    mediaWorkingDebug["fps"] = media->working->fps;
    mediaWorkingDebug["completedFrames"] = media->working->completedFrames;
    mediaWorkingDebug["quality"] = media->working->quality;
    mediaWorkingDebug["bitrate"] = media->working->bitrate;

    mediaDebug["file"] = mediaFileDebug;
    mediaDebug["video"] = mediaVideoDebug;
    mediaDebug["working"] = mediaWorkingDebug;

    child["converting"].push_back(mediaDebug);

    this->converting.pop();
    t_queue.push(media);
  }

  this->converting = t_queue;
  t_queue = std::queue<Media *>();

  // pending file
  while (!this->pending.empty()) {
    Media *media = this->pending.front();
    nlohmann::json mediaDebug;
    nlohmann::json mediaFileDebug;
    nlohmann::json mediaVideoDebug;
    nlohmann::json mediaWorkingDebug;

    mediaDebug["activity"] =
        EnumToStringFactory::get(media->getActivity()).getName();
    mediaDebug["started"] = media->started;
    mediaDebug["ended"] = media->ended;
    mediaDebug["ffmpegArguments"] = media->ffmpegArguments->build();

    mediaFileDebug["originalFileNameExt"] = media->file->originalFileNameExt;
    mediaFileDebug["originalFullPath"] = media->file->originalFullPath;
    mediaFileDebug["conversionName"] = media->file->conversionName;
    mediaFileDebug["conversionNameExt"] = media->file->conversionNameExt;
    mediaFileDebug["conversionFolderPath"] = media->file->conversionFolderPath;
    mediaFileDebug["conversionFilePath"] = media->file->conversionFilePath;
    mediaFileDebug["ext"] = media->file->ext;
    mediaFileDebug["size"] = media->file->size;
    mediaFileDebug["newSize"] = media->file->newSize;
    mediaFileDebug["cwd"] = media->file->cwd;
    mediaFileDebug["quality"] = media->file->quality;
    mediaFileDebug["series"] = media->file->series;
    mediaFileDebug["season"] = media->file->season;

    mediaVideoDebug["fps"] = media->video->fps;
    mediaVideoDebug["totalFrames"] = media->video->totalFrames;
    mediaVideoDebug["subtitleProvider"] = media->video->subtitleProvider;
    mediaVideoDebug["width"] = media->video->width;
    mediaVideoDebug["height"] = media->video->height;
    mediaVideoDebug["ratio"] = media->video->ratio;
    mediaVideoDebug["convertedWidth"] = media->video->convertedWidth;
    mediaVideoDebug["convertedHeight"] = media->video->convertedHeight;
    mediaVideoDebug["convertedResolution"] = media->video->convertedResolution;
    mediaVideoDebug["crop"] = media->video->crop;
    mediaVideoDebug["crf"] = media->video->crf;

    mediaWorkingDebug["fps"] = media->working->fps;
    mediaWorkingDebug["completedFrames"] = media->working->completedFrames;
    mediaWorkingDebug["quality"] = media->working->quality;
    mediaWorkingDebug["bitrate"] = media->working->bitrate;

    mediaDebug["file"] = mediaFileDebug;
    mediaDebug["video"] = mediaVideoDebug;
    mediaDebug["working"] = mediaWorkingDebug;

    child["pending"].push_back(mediaDebug);
    this->pending.pop();
    t_queue.push(media);
  }

  this->pending = t_queue;

  return child;
}
