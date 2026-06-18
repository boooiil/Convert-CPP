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
#include "../settings/arguments/IntegerArgument.h"
#include "../settings/enums/Activity_N.h"
#include "media/Media.h"
#include "src/program/context/RuntimeEnvironment.h"
#include "src/program/settings/enums/Command_N.h"
#include "src/utils/logging/Logger.h"

std::vector<std::thread> workerThreads;

// template <typename T>
// typename ArgumentRegistry::getTFn<T> get =
// Child::settings->argumentRegistry.get<T>;

Child::Child(RuntimeEnvironment &run_env, std::shared_ptr<Arguments> arguments)
    : endable(false), run_env(run_env), arguments(arguments) {}

void Child::prepare(std::vector<std::string> &args) {
  // initialize settings

  std::random_device rd;  // Seed for random number generator
  std::mt19937 gen(rd()); // Standard mersenne_twister_engine seeded with rd()
  uuids::uuid_random_generator generator(
      gen); // Pass the generator to uuid_random_generator
  this->id = generator();

  std::vector<std::filesystem::directory_entry> files =
      DirectoryUtils::getFilesInDirectory(args[0],
                                          std::vector{".mp4", ".mkv", ".avi"});

  for (std::filesystem::directory_entry file : files) {
    std::string cwd = file.path().parent_path().string();
    std::string filename = file.path().filename().string();

    Media *media = new Media(*this->arguments, this->id, filename, cwd);
    media->getFile().naming.rename();

    bool fs_exists =
        std::filesystem::exists(media->getFile().naming.conversion_folder_path);
    bool is_info = arguments->argumentRegistry.get<Command_N::INFO>().get();

    LOG_DEBUG(Logger::Priority::INFO, "fs exists?",
              fs_exists ? "true" : "false", "is info?",
              is_info ? "true" : "false");

    // if the conversion folder path does not exist, and we are not
    // trying to get the info, create it
    if (!fs_exists && !is_info) {
      LOG_DEBUG(Logger::Priority::INFO, "Creating directory: ",
                media->getFile().naming.conversion_folder_path);
      std::filesystem::create_directory(
          media->getFile().naming.conversion_folder_path);
    } else {
      LOG_DEBUG(Logger::Priority::INFO, "Directory exists or info:",
                media->getFile().naming.conversion_folder_path);
    }

    LOG_DEBUG(Logger::Priority::INFO, "Adding media to pending queue: ",
              media->getFile().naming.original_name_ext);
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
//       !this->processSettings->argumentParser->argumentRegistry.get<FlagArgument>("-i")->get())
//       { LOG_DEBUG(Logger::Priority::INFO,"Creating directory: ",
//       media->file->conversionFolderPath);
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

  IntegerArgument setAmount =
      arguments->argumentRegistry.get<Command_N::AMOUNT>();

  LOG_DEBUG(Logger::Priority::INFO, "C:" + std::to_string(currentAmount),
            "W:" + setAmount.toString(),
            "T:" +
                std::to_string(this->pending.size() + this->converting.size()));

  if ((currentAmount < (int)setAmount) && !this->pending.empty()) {
    Media *media = this->pending.front();

    // if there are no media files waiting
    // and the current amount of converting media is 0
    // then exit the program
    if (!media->isWaiting()) {
      LOG_DEBUG(Logger::Priority::INFO, "Media is not waiting:",
                media->getFile().naming.original_name_ext,
                Activity_N::definition(media->getActivity()));
      if (currentAmount == 0) {
        this->setEndable(true);
        this->setCompleted(true);
        // Program::stopFlag = true;
      }
    } else {
      LOG_DEBUG(Logger::Priority::INFO, "Queued media for encoding:",
                media->getFile().naming.original_name_ext,
                Activity_N::definition(media->getActivity()));
      media->setActivity(Activity_N::WAITING_STATISTICS);

      media->started = TimeUtils::getEpoch();

      this->converting.push(media);
      this->pending.pop();
    }

    this->setEndable(true);
  }

  // error if there are more converting than allowed
  if (currentAmount > (int)setAmount) {
    LOG(Logger::Priority::INFO,
        LogColor::fgRed(
            "CURRENT TRANSCODES ARE GREATER THAN THE ALLOWED AMOUNT."));

    LOG(Logger::Priority::INFO,
        LogColor::fgRed("CURRENT ALLOWED AMOUNT: " + setAmount.toString()));

    LOG(Logger::Priority::INFO,
        LogColor::fgRed("CURRENT QUEUE: " + setAmount.toString()));

    // iterate over converting
    while (!this->converting.empty()) {
      Media *value = this->converting.front();
      LOG(Logger::Priority::INFO,
          LogColor::fgRed("CURRENT FILE: " +
                          value->getFile().naming.conversion_name));
      this->converting.pop();
    }

    this->setEndable(true);
  }

  // temp queue for conversion iteration
  std::queue<Media *> t_queue;

  // iterate over converting media
  while (!this->converting.empty()) {
    Media *media = this->converting.front();

    LOG_DEBUG(Logger::Priority::INFO, media->getFile().naming.original_name_ext,
              Activity_N::definition(media->getActivity()));

    if (!media->isProcessing()) {
      LOG_DEBUG(Logger::Priority::INFO, "Media is not processing:",
                media->getFile().naming.original_name_ext,
                Activity_N::definition(media->getActivity()));

      if (media->isWaitingToStatistics()) {
        LOG_DEBUG(Logger::Priority::INFO, "Media is waiting for statistics:",
                  media->getFile().naming.original_name_ext);
        media->doStatistics();
      } else if (media->isWaitingToConvert()) {
        LOG_DEBUG(Logger::Priority::INFO, "Media is waiting for conversion:",
                  media->getFile().naming.original_name_ext);
        media->buildFFmpegArguments(false);
        workerThreads.emplace_back([media]() { media->doConversion(); });
      } else if (media->isWaitingToValidate()) {
        LOG_DEBUG(Logger::Priority::INFO, "Media is waiting for validation:",
                  media->getFile().naming.original_name_ext);
        workerThreads.emplace_back([media]() { media->doValidation(); });
      }
    }

    if (media->hasFailed() || media->hasFinished()) {
      // todo: again, chrono stuff
      media->ended = TimeUtils::getEpoch();

      LOG_DEBUG(Logger::Priority::INFO,
                "Media ended:", media->getFile().naming.conversion_name);
      this->pending.push(media);
      LOG_DEBUG(Logger::Priority::INFO, "pending size after finish:",
                std::to_string(this->pending.size()));
    } else {
      t_queue.push(media);
    }
    this->converting.pop();
  }

  LOG_DEBUG(Logger::Priority::INFO,
            "t_queue size:", std::to_string(t_queue.size()));
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
  LOG_DEBUG(Logger::Priority::INFO, "Ending child runner.");
  LOG_DEBUG(Logger::Priority::INFO,
            "Expected to delete { pending[], converting[], settings }.");
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

    LOG_DEBUG(Logger::Priority::INFO, "Deconstructing media in:",
              media->getFile().naming.original_name_ext);

    delete media;
  }

  // iterate over converting
  while (!this->converting.empty()) {
    Media *media = this->converting.front();
    this->converting.pop();

    LOG_DEBUG(Logger::Priority::INFO, "Deconstructing media in:",
              media->getFile().naming.original_name_ext);

    delete media;
  }

  // if (this->arguments != nullptr) {
  //   LOG_DEBUG(Logger::Priority::INFO,"Deconstructing child options.");
  //   delete this->arguments;
  // }

  /*if (this->processSettings != nullptr) {
    LOG_DEBUG(Logger::Priority::INFO,"Deconstructing settings.");
    delete this->processSettings;
  }*/
}

void Child::fromJSON(const nlohmann::json &json) {}

nlohmann::json Child::toJSON() {
  using namespace nlohmann;

  json child;

  std::queue<Media *> t_queue;

  child["id"] = uuids::to_string(this->id);
  child["pending"] = nlohmann::json::array();
  child["converting"] = nlohmann::json::array();

  // converting file
  while (!this->converting.empty()) {
    Media *media = this->converting.front();

    nlohmann::json mediaDebug = media->toJSON();

    child["converting"].push_back(mediaDebug);

    this->converting.pop();
    t_queue.push(media);
  }

  this->converting = t_queue;
  t_queue = std::queue<Media *>();

  // pending file
  while (!this->pending.empty()) {
    Media *media = this->pending.front();
    nlohmann::json mediaDebug = media->toJSON();

    child["pending"].push_back(mediaDebug);
    this->pending.pop();
    t_queue.push(media);
  }

  this->pending = t_queue;

  return child;
}
