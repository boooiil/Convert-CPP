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
#include "../settings/arguments/ArgumentRegistry.h"
#include "../settings/arguments/EnumArgument.h"
#include "../settings/arguments/FlagArgument.h"
#include "../settings/arguments/IntegerArgument.h"
#include "../settings/enums/LoggingOptions.h"
#include "nlohmann/json.hpp"

NTicker::NTicker() : endable(true) {
  this->display = nullptr;
  this->runner = nullptr;
}

void NTicker::determineNextAction(std::vector<std::string>& args) {
  ArgumentRegistry* program_arg_reg =
      Program::settings->programOptions->argumentRegistry;

  if (program_arg_reg->get_t<FlagArgument>(Command::HELP)->get()) {
    Help::printHelp();
    Program::stopFlag = true;
  } else if (program_arg_reg->get_t<FlagArgument>(Command::INFO)->get()) {
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
//   if (arguments->argumentRegistry.get_t<FlagArgument>("-parent")->get()) {
//     LOG_DEBUG("Running as parent.");
//     this->display = new ParentDisplay();
//     this->runner = new Parent();
//   }
//   else {
//     LOG_DEBUG("Running as child.");
//     this->display = new ChildDisplay();
//     this->runner = new Child();
//   }
//
//   this->determineNextAction(arguments->args);
// }

void NTicker::prepare(std::vector<std::string>& args) {
  // use parent display
  ArgumentRegistry* program_arg_reg =
      Program::settings->programOptions->argumentRegistry;

  if (program_arg_reg->get_t<FlagArgument>(Command::PARENT)->get()) {
    LOG_DEBUG("Running as parent.");
    this->display = new ParentDisplay();
    this->runner = new Parent();
  } else {
    LOG_DEBUG("Running as child.");
    this->display = new ChildDisplay();
    this->runner = new Child();
  }

  this->determineNextAction(args);
}

void NTicker::run() {
  ArgumentRegistry* program_arg_reg =
      Program::settings->programOptions->argumentRegistry;

  if (program_arg_reg->get_t<FlagArgument>(Command::INFO)->get()) {
    this->display->printInformation(this, this->runner);
    return;
  }

  while (!Program::stopFlag) {
    this->runner->run();

    LoggingOptions log_option =
        program_arg_reg
            ->get_t<EnumArgument<LoggingOptions>>(Command::LOGGINGOPTIONS)
            ->get();

    switch (log_option) {
      case LoggingOptions::DEBUG:
      case LoggingOptions::JSON_DEBUG:
        this->display->printDebug();
        break;
      case LoggingOptions::JSON:
        this->display->printJSON();
        break;
      default:
        this->display->print();
        break;
    }

    std::this_thread::sleep_for(
        std::chrono::milliseconds((int)*program_arg_reg->get_t<IntegerArgument>(
            Command::DISPLAYREFRESH)));
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
  LOG_DEBUG("Ending ticker.");
  LOG_DEBUG("Expected to delete { display, runner }.");

  if (this->display != nullptr) {
    LOG_DEBUG("Deleting display.");
    delete this->display;
  }

  if (this->runner != nullptr) {
    // while (!this->runner->isEndable()) {
    //   LOG_DEBUG("Waiting for runner to end.");

    //  std::this_thread::sleep_for(std::chrono::milliseconds(500));
    // while (!Program::ticker->runner->isEndable()
    // wait for runner to end
    //}
    this->runner->end();

    LOG_DEBUG("Deleting runner.");
    delete this->runner;
  }
}

void NTicker::setEndable(bool flag) {
  LOG_DEBUG("Parent has been set as endable:",
            this->endable ? "True" : "False");
  this->endable = flag;
}

bool NTicker::isEndable() { return this->endable; }

void NTicker::fromJSON(nlohmann::json) {
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
