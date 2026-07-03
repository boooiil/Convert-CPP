#include "NTicker.h"

#include <chrono>
#include <thread>
#include <vector>

#include "../../utils/logging/Logger.h"
#include "../Program.h"
#include "../child/Child.h"
#include "../child/display/ChildDisplay.h"
#include "../parent/Parent.h"
#include "../parent/display/ParentDisplay.h"
#include "../settings/Help.h"
#include "../settings/arguments/IntegerArgument.h"
#include "nlohmann/json.hpp"
#include "src/program/context/Arguments.h"
#include "src/program/context/RuntimeEnvironment.h"
#include "src/program/settings/enums/Command_N.h"
#include "src/program/settings/enums/LogFormat_N.h"

NTicker::NTicker(RuntimeEnvironment &run_env,
                 std::shared_ptr<Arguments> &arguments)
    : runner(nullptr), endable(true), display(nullptr), arguments(arguments),
      run_env(run_env) {};

void NTicker::determineNextAction(std::vector<std::string> &args) {
  ArgumentRegistry &argumentRegistry = this->arguments->argumentRegistry;

  if (argumentRegistry.get_p<Command_N::HELP>()) {
    Help::print(*arguments);
    Program::stopFlag = true;
  } else if (argumentRegistry.get_p<Command_N::INFO>()) {
    // print information
    this->runner->prepare(args);
    this->display->printInformation(this, this->runner);
    Program::stopFlag = true;
  } else {
    // prepare runner
    this->runner->prepare(args);
  }
  // if (print help) display help
  // if (print information) print information
  //   print media information for child
  //   print directory informattion for parent
  // else start ticker
  // end ticker
}

// void NTicker::prepare(ArgumentParser* arguments) {
//   // use parent display
//   if (arguments->argumentRegistry.get_p<FlagArgument>("-parent")->get()) {
//     LOG_DEBUG(Logger::Priority::INFO,"Running as parent.");
//     this->display = new ParentDisplay();
//     this->runner = new Parent();
//   }
//   else {
//     LOG_DEBUG(Logger::Priority::INFO,"Running as child.");
//     this->display = new ChildDisplay();
//     this->runner = new Child();
//   }
//
//   this->determineNextAction(arguments->args);
// }

void NTicker::prepare(std::vector<std::string> &args) {
  // use parent display
  ArgumentRegistry &argumentRegistry = this->arguments->argumentRegistry;

  if (argumentRegistry.get_p<Command_N::PARENT>()) {
    LOG_DEBUG(Logger::Priority::INFO, "Running as parent.");
    this->display = new ParentDisplay();
    this->runner = new Parent(run_env, *arguments);
  } else {
    LOG_DEBUG(Logger::Priority::INFO, "Running as child.");
    this->display = new ChildDisplay();
    this->runner = new Child(run_env, arguments);
  }

  this->determineNextAction(args);
}

void NTicker::run() {
  ArgumentRegistry &argumentRegistry = this->arguments->argumentRegistry;

  if (argumentRegistry.get_p<Command_N::INFO>()) {
    this->display->printInformation(this, this->runner);
    return;
  }

  while (!Program::stopFlag) {
    this->runner->run();

    if (this->runner->isCompleted() || Program::stopFlag) {
      // TODO: try to get away from stopflag
      Program::stopFlag = true;
      break;
    }

    LogFormat_N::LogFormat log_option =
        argumentRegistry.get_p<Command_N::LOGGINGOPTIONS>().get();

    switch (log_option) {
    case LogFormat_N::DEBUG:
    case LogFormat_N::JSON_DEBUG:
      this->display->printDebug(*this);
      break;
    case LogFormat_N::VERBOSE:
    case LogFormat_N::JSON_VERBOSE:
      this->display->printDebug(*this);
      break;
    case LogFormat_N::JSON:
      this->display->printJSON();
      break;
    default:
      this->display->print(*this);
      break;
    }

    std::this_thread::sleep_for(std::chrono::milliseconds(
        (int)argumentRegistry.get_p<Command_N::DISPLAYREFRESH>().get()));
  }
  // if (Program::settings->argumentParser->isParent) {
  //   // parent display
  //   this->display->print();
  // } else {
  //   // child display
  //   this->display->print();
  // }
}

void NTicker::end() {
  LOG_DEBUG(Logger::Priority::INFO, "Ending ticker.");
  LOG_DEBUG(Logger::Priority::INFO, "Expected to delete { display, runner }.");

  if (this->display != nullptr) {
    LOG_DEBUG(Logger::Priority::INFO, "Deconstructing display.");
    delete this->display;
  }

  if (this->runner != nullptr) {
    // while (!this->runner->isEndable()) {
    //   LOG_DEBUG(Logger::Priority::INFO,"Waiting for runner to end.");

    //  std::this_thread::sleep_for(std::chrono::milliseconds(500));
    // while (!Program::ticker->runner->isEndable()
    // wait for runner to end
    //}
    this->runner->end();

    LOG_DEBUG(Logger::Priority::INFO, "Deconstructing runner.");
    delete this->runner;
  }
}

void NTicker::fromJSON(const nlohmann::json &json) {
  // read json information
}

nlohmann::json NTicker::toJSON() {
  using namespace nlohmann;

  json program;

  if (this->runner == nullptr) {
    program["Runner"] = {};
  } else {
    program["Runner"] = this->runner->toJSON();
  }

  return program;
}

void NTicker::writeDebug() {
  // write debug information
}
