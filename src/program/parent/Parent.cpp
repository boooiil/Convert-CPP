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

Parent::Parent(RuntimeEnvironment &run_env, Arguments &arguments)
    : run_env(run_env), arguments(arguments) {
  LOG_DEBUG(Logger::Priority::INFO, "Constructing parent.");
}

Parent::~Parent(void) {
  LOG_DEBUG(Logger::Priority::INFO, "Deconstructing parent.");
  while (!this->pending.empty()) {
    Child *child = this->pending.front();
    this->pending.pop();

    LOG_DEBUG(Logger::Priority::INFO,
              "Deconstructing child process in:", child->getArguments().CWD,
              "with uuid:", child->id);

    child->end();
    delete child;
  }
  while (!this->converting.empty()) {
    Child *child = this->converting.front();
    this->converting.pop();

    LOG_DEBUG(Logger::Priority::INFO,
              "Deconstructing child process in:", child->getArguments().CWD,
              "with uuid:", child->id);

    child->end();
    delete child;
  }
  // if (this->options != nullptr) {
  //   LOG_DEBUG(Logger::Priority::INFO,"Deconstructing ParentOptions.");
  //   delete this->options;
  // }
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

void Parent::prepare(std::vector<std::string> &args) {

  std::vector<std::filesystem::directory_entry> files;

#ifdef _WIN32
  files = DirectoryUtils::findFileInSubdir({"convert.exe"});
#else
  files = DirectoryUtils::findFileInSubdir({"convert"});
#endif

  for (std::filesystem::directory_entry file : files) {
    std::filesystem::path path = file.path();
    std::string filename = file.path().filename().string();

    std::vector<std::string> n_args = this->getArgs(file);

    Child *child = new Child(run_env, Arguments::parse(run_env, n_args));

    child->prepare(n_args);
    this->pending.push(child);

    this->setCompleted(true);

    // get args, init settings on child

    // ChildProcess* child = new ChildProcess(path, filename);
    // child->getArgs();
    //
    // this->converting.push(child);
  };
}

void Parent::run(void) { LOG(Logger::Priority::INFO, "PARENT RUN"); }

void Parent::end(void) {
  LOG_DEBUG(Logger::Priority::INFO, "Ending runner.");
  LOG_DEBUG(Logger::Priority::INFO, "Expected to delete { }.");
}

std::vector<std::string>
Parent::getArgs(std::filesystem::directory_entry file) {
  std::cout << file.path().parent_path() << " arguments: ";
  std::string input = "";

  std::getline(std::cin, input);

  input = file.path().parent_path().string() + " -lf json " + input;

  return ListUtils::splitv(input, " ");
}

void Parent::fromJSON(const nlohmann::json &json) {}

nlohmann::json Parent::toJSON(void) {
  nlohmann::json parent;
  nlohmann::json converting_json = nlohmann::json::array();
  nlohmann::json pending_json = nlohmann::json::array();

  // j["ChildProcesses"] = nlohmann::json::array();

  std::queue<Child *> t_queue;

  while (!this->pending.empty()) {
    Child *child = this->pending.front();
    this->pending.pop();

    LOG_DEBUG(Logger::Priority::INFO,
              "parent json: ", child->getArguments().CWD);

    converting_json.push_back(child->toJSON());

    t_queue.push(child);
  }

  this->pending = t_queue;

  t_queue = std::queue<Child *>();

  while (!this->converting.empty()) {
    Child *child = this->converting.front();
    this->converting.pop();

    LOG_DEBUG(Logger::Priority::INFO,
              "parent json: ", child->getArguments().CWD);

    pending_json.push_back(child->toJSON());

    t_queue.push(child);
  }

  this->converting = t_queue;

  parent["converting"] = converting_json;
  parent["pending"] = pending_json;

  return parent;
}
