#include "Parent.h"

#include <filesystem>
#include <iostream>
#include <nlohmann/json.hpp>
#include <queue>
#include <string>
#include <vector>

#include "../../utils/DirectoryUtils.h"
#include "../../utils/ListUtils.h"
#include "../../utils/logging/Logger.h"
#include "../Program.h"
#include "../child/Child.h"

/**
 * Parent - Gather all files and queue into  ChildProcess.
 * Child process - Run convert and parse
 *   - Parse converting and pending
 *     - if not in map, add it
 *     - else, update non static fields
 *
 */

/**
 * ALT:
 * Parent - Gather all directories and search for required file types.
 *  - If found, create a child at the given path.
 *  - Add the child to the converting queue.
 *  - Continue for other children.
 *
 */

Parent::~Parent(void) {
  LOG_DEBUG("Deconstructing parent.");
  while (!this->pending.empty()) {
    Child* child = this->pending.front();
    this->pending.pop();

    LOG_DEBUG("Deleting child process in:",
              Program::settings->childOptionsMap[child->id]->CWD);

    delete child;
  }
  while (!this->converting.empty()) {
    Child* child = this->converting.front();
    this->converting.pop();

    LOG_DEBUG("Deleting child process in:",
              Program::settings->childOptionsMap[child->id]->CWD);

    delete child;
  }
}

// void Parent::prepare(ArgumentParser* arguments) {
//   this->arguments = arguments;
//
//   std::vector<std::filesystem::directory_entry> files;
//
// #ifdef _WIN32
//   files = DirectoryUtils::findFileInSubdir({ "convert.exe" });
// #else
//   files = DirectoryUtils::findFileInSubdir({ "convert" });
// #endif
//
//   for (std::filesystem::directory_entry file : files) {
//     std::string path = file.path().string();
//     std::string filename = file.path().filename().string();
//
//     Child* child = new Child();
//
//     std::vector<std::string> args = this->getArgs(file);
//
//     child->prepare(args);
//     this->converting.push(child);
//   }
// }

void Parent::prepare(std::vector<std::string>& args) {
  ParentOptions& parentOptions = *Program::settings->parentOptions;

  parentOptions.prepare();
  parentOptions.parse(args);
  parentOptions.validate();

  std::vector<std::filesystem::directory_entry> files;

#ifdef _WIN32
  files = DirectoryUtils::findFileInSubdir({"convert.exe"});
#else
  files = DirectoryUtils::findFileInSubdir({"convert"});
#endif

  for (std::filesystem::directory_entry file : files) {
    std::string path = file.path().string();
    std::string filename = file.path().filename().string();

    Child* child = new Child();

    std::vector<std::string> args = this->getArgs(file);

    child->prepare(args);
    this->converting.push(child);

    // get args, init settings on child

    // ChildProcess* child = new ChildProcess(path, filename);
    // child->getArgs();
    //
    // this->converting.push(child);
  };
}

void Parent::run(void) {}

void Parent::end(void) {
  LOG_DEBUG("Ending runner.");
  LOG_DEBUG("Expected to delete { }.");
}

void Parent::setEndable(bool flag) {
  LOG_DEBUG("Parent has been set as endable:",
            this->endable ? "True" : "False");
  this->endable = flag;
}

bool Parent::isEndable(void) { return this->endable; }

std::vector<std::string> Parent::getArgs(
    std::filesystem::directory_entry file) {
  std::cout << file.path().parent_path() << " arguments: ";
  std::string input = "";

  std::getline(std::cin, input);

  input.append(file.path().parent_path().string() + " ");
  input.append("-lf json ");

  return ListUtils::splitv(input, " ");
}

void Parent::fromJSON(nlohmann::json) {}

nlohmann::json Parent::toJSON(void) {
  nlohmann::json parent;
  nlohmann::json converting_json = nlohmann::json::array();
  nlohmann::json pending_json = nlohmann::json::array();

  // j["ChildProcesses"] = nlohmann::json::array();

  std::queue<Child*> t_queue;

  while (!this->pending.empty()) {
    Child* child = this->pending.front();
    this->pending.pop();

    LOG_DEBUG("parent json: ",
              Program::settings->childOptionsMap[child->id]->CWD);

    converting_json.push_back(child->toJSON());

    t_queue.push(child);
  }

  this->pending = t_queue;

  t_queue = std::queue<Child*>();

  while (!this->converting.empty()) {
    Child* child = this->converting.front();
    this->converting.pop();

    LOG_DEBUG("parent json: ",
              Program::settings->childOptionsMap[child->id]->CWD);

    pending_json.push_back(child->toJSON());

    t_queue.push(child);
  }

  this->converting = t_queue;

  parent["converting"] = converting_json;
  parent["pending"] = pending_json;

  return parent;
}
