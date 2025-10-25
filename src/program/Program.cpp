#include "Program.h"

#include <fstream>
#include <iosfwd>
#include <nlohmann/json.hpp>
#include <string>
#include <vector>

#include "../utils/ListUtils.h"
#include "../utils/logging/LogColor.h"
#include "../utils/logging/Logger.h"
#include "generics/JSONSerializableRunner.h"
#include "settings/Settings.h"
#include "settings/arguments/EnumArgument.h"
#include "settings/enums/EnumToStringFactory.h"
#include "settings/enums/LoggingOptions.h"
#include "ticker/NTicker.h"

JSONSerializableRunner *Program::ticker = nullptr;
// Log* Program::log = nullptr;
Settings *Program::settings = nullptr;
bool Program::stopFlag = false;

Program::Program(void) : endable(true) {}
Program::~Program(void) {
  // should always call end
  // this->end();
}

void Program::prepare(std::vector<std::string> &args) {
  // Program::log = new Log();
  EnumToStringFactory::init();

  Program::settings = new Settings();
  Program::settings->programOptions->prepare();
  Program::settings->programOptions->parse(args);

  switch (Program::settings->programOptions->argumentRegistry
              ->get_t<EnumArgument<LoggingOptions>>(Command::LOGGINGOPTIONS)
              ->get()) {
  case LoggingOptions::DEBUG:
  case LoggingOptions::JSON_DEBUG:
    Logger::debug_flag = true;
    break;
  case LoggingOptions::VERBOSE:
  case LoggingOptions::JSON_VERBOSE:
    Logger::debug_flag = true;
    // set verbose
    break;
  default:
    Logger::debug_flag = false;
    break;
  };

  Program::settings->programOptions->gatherSystemDetails();
  Program::settings->programOptions->validate();

  Program::ticker = new NTicker();
  if (!stopFlag)
    Program::ticker->prepare(args);
}

void Program::prepare(int argc, char *argv[]) {
  (void)argc;
  std::vector<std::string> args = ListUtils::toStrVector(argv);

  args.erase(args.begin());
  args.insert(args.begin(), std::filesystem::current_path().string());

  Program::prepare(args);
}

void Program::run() {
  if (!stopFlag)
    Program::ticker->run();
}

void Program::end(void) {
  LOG_DEBUG("Ending program.");
  LOG_DEBUG("Expected to delete { log, ticker, settings, enum registry }.");

  Program::stopFlag = true;

  if (Logger::debug_flag) {
    LOG_DEBUG("Saving debug file.");

    std::ofstream oFile("container_debug.json");

    if (!oFile.is_open()) {
      LOG(LogColor::fgRed("Failed to open debug file->"));
      return;
    }

    oFile << Program::toJSON().dump(4);
    oFile.close();
  }

  if (Program::ticker != nullptr) {
    LOG_DEBUG("Deleting ticker.");
    Program::ticker->end();
    delete Program::ticker;
  }

  /*if (Program::log != nullptr) {
    Program::log->end();
    LOG_DEBUG("Deleting log.");
    delete Program::log;
  }*/

  if (Program::settings != nullptr) {
    LOG_DEBUG("Deleting settings.");
    delete Program::settings;
  }

  EnumToStringFactory::end();
  // TODO: end needs to exit program
  // add ticker
}

void Program::setEndable(bool flag) {
  LOG_DEBUG("Program has been set as endable:",
            Program::stopFlag ? "True" : "False");
  Program::stopFlag = flag;
}

bool Program::isEndable() { return Program::stopFlag; }

void Program::fromJSON(nlohmann::json program) { (void)program; }

nlohmann::json Program::toJSON() {
  using namespace nlohmann;

  json program;
  json settings_json = Program::settings->toJSON();

  program["Settings"] = settings_json;

  if (Program::ticker != nullptr) {
    program["Ticker"] = Program::ticker->toJSON();
  }

  return program;
}
