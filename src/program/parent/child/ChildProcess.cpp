#include "ChildProcess.h"

#include <iostream>
#include <nlohmann/json.hpp>
#include <string>

#include "../../../utils/logging/Logger.h"
#include "../../settings/enums/Activity.h"
#include "ChildProcessConversion.h"
#include "src/program/settings/enums/EnumToStringFactory.h"
#include "src/utils/StringUtils.h"

ChildProcess::ChildProcess(std::string path, std::string filename)
    : path(path), filename(filename), pid(-1), endable(true), ended(0),
      started(0), activity(Activity::WAITING) {}

ChildProcess::~ChildProcess(void) {}

/**
 * My thinking for this file:
 *
 * Store the media in an object,
 *
 *   MediaMap[media.file.name] = *Media
 *
 *   It is known that the size of the set will be constant.
 *
 * there is no activity that happens on these objects
 * it is updated when the return value gets updated.
 *
 *  we will need to only update select fields from each media object
 *  when we do child process conversion
 *
 */

Activity ChildProcess::getActivity() { return this->activity; }

void ChildProcess::getArgs(void) {
  std::cout << this->path << " arguments: ";
  std::getline(std::cin, this->args);

  this->args = "-lf json " + this->args;
}

const bool ChildProcess::isProcessing() {
  return ChildProcess::activity == Activity::STATISTICS ||
         ChildProcess::activity == Activity::CONVERT ||
         ChildProcess::activity == Activity::VALIDATE;
}

const bool ChildProcess::hasFailed() {
  return StringUtils::contains(
      EnumToStringFactory::get(ChildProcess::activity).getName(), "failed");
}

const bool ChildProcess::hasFinished() {
  return this->activity == Activity::FINISHED;
}

const bool ChildProcess::isWaiting() {
  return this->activity == Activity::WAITING;
}

const bool ChildProcess::isWaitingToConvert() {
  return this->activity == Activity::WAITING_CONVERT;
}

void ChildProcess::doConversion(void) {
  this->activity = Activity::CONVERT;

  LOG_DEBUG("Running conversion for:", this->path);

  ChildProcessConversion conversion(this);
  conversion.start(this->path + "/" + this->filename + " " + this->args);
}

void ChildProcess::fromJSON(nlohmann::json childProcess) {
  nlohmann::json runner = childProcess["Runner"];

  for (nlohmann::json mediaFile : runner["converting"]) {
    this->fromJSON(mediaFile);
  }

  for (nlohmann::json mediaFile : runner["pending"]) {
    this->fromJSON(mediaFile);
  }
}

nlohmann::json ChildProcess::toJSON(void) {
  nlohmann::json childProcess;

  childProcess["path"] = this->path;
  childProcess["filename"] = this->filename;
  childProcess["args"] = this->args;

  return childProcess;
}
