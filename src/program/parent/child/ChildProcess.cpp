#include "ChildProcess.h"

#include <iostream>
#include <nlohmann/json.hpp>
#include <string>

#include "../../../utils/logging/Logger.h"
#include "../../settings/enums/Activity_N.h"
#include "ChildProcessConversion.h"

ChildProcess::ChildProcess(std::string path, std::string filename)
    : pid(-1), started(0), ended(0), path(path), filename(filename),
      activity(Activity_N::WAITING), endable(true) {}

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

Activity_N::Activity ChildProcess::getActivity() { return this->activity; }

void ChildProcess::getArgs(void) {
  std::cout << this->path << " arguments: ";
  std::getline(std::cin, this->args);

  this->args = "-lf json " + this->args;
}

const bool ChildProcess::isProcessing() {
  return Activity_N::isProcessing(this->activity);
}

const bool ChildProcess::hasFailed() {
  return Activity_N::isFailed(this->activity);
}

const bool ChildProcess::hasFinished() {
  return this->activity == Activity_N::FINISHED;
}

const bool ChildProcess::isWaiting() {
  return this->activity == Activity_N::WAITING;
}

const bool ChildProcess::isWaitingToConvert() {
  return this->activity == Activity_N::WAITING_CONVERT;
}

void ChildProcess::doConversion(void) {
  this->activity = Activity_N::CONVERT;

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
