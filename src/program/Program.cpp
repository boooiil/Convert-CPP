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
#include "settings/enums/LogFormat_N.h"
#include "src/program/context/RuntimeEnvironment.h"
#include "src/utils/logging/Logger.h"
#include "ticker/NTicker.h"

// Log* Program::log = nullptr;
std::atomic<bool> Program::stopFlag = false;

Program::Program(void) : endable(true) {
  this->runtimeEnv = RuntimeEnvironment::detect();
}
Program::~Program(void) {
  // should always call end
  // this->end();
}

void Program::prepare(std::vector<std::string> &args) {
  // Program::log = new Log();

  this->arguments = Arguments::parse(*this->runtimeEnv, args);

  switch (this->arguments->argumentRegistry.get_p<Command_N::LOGGINGOPTIONS>()
              .get()) {
  case LogFormat_N::DEBUG:
  case LogFormat_N::JSON_DEBUG:
    Logger::debug_flag = true;
    Logger::verbose_flag = false;
    std::cout << "Attach debugger then press enter to continue...";
    std::cin.get();
    break;
  case LogFormat_N::VERBOSE:
  case LogFormat_N::JSON_VERBOSE:
    Logger::debug_flag = true;
    Logger::verbose_flag = true;
    std::cout << "Attach debugger then press enter to continue...";
    std::cin.get();
    break;
  default:
    Logger::debug_flag = false;
    Logger::verbose_flag = false;
    break;
  };

  this->arguments->validate();

  this->ticker = new NTicker(*this->runtimeEnv, this->arguments);
  if (!stopFlag)
    this->ticker->prepare(args);
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
    this->ticker->run();
}

void Program::end(void) {
  LOG_DEBUG(Logger::Priority::INFO, "Ending program.");
  LOG_DEBUG(Logger::Priority::INFO,
            "Expected to delete { log, ticker, settings, enum registry }.");

  Program::stopFlag = true;

  if (Logger::debug_flag) {
    LOG_DEBUG(Logger::Priority::INFO, "Saving debug file.");

    std::ofstream oFile("container_debug.json");

    if (!oFile.is_open()) {
      LOG(Logger::Priority::INFO,
          LogColor::fgRed("Failed to open debug file->"));
      return;
    }

    oFile << Program::toJSON().dump(4);
    oFile.close();
  }

  if (this->ticker != nullptr) {
    LOG_DEBUG(Logger::Priority::INFO, "Deconstructing ticker.");
    this->ticker->end();
    delete this->ticker;
  }

  /*if (Program::log != nullptr) {
    Program::log->end();
    LOG_DEBUG(Logger::Priority::INFO,"Deconstructing log.");
    delete Program::log;
  }*/

  // if (this->options != nullptr) {
  //   LOG_DEBUG(Logger::Priority::INFO,"Deconstructing ProgramOptions.");
  //   delete this->options;
  // }

  if (this->runtimeEnv != nullptr) {
    LOG_DEBUG(Logger::Priority::INFO, "Deconstructing RuntimeEnvironment.");
    delete this->runtimeEnv;
  }

  // TODO: end needs to exit program
  // add ticker
}

void Program::setStopping(bool flag) {
  LOG_DEBUG(Logger::Priority::INFO,
            "Program has been set as endable:", flag ? "True" : "False");
  Program::stopFlag = flag;
}

bool Program::isStopping() { return Program::stopFlag; }

void Program::fromJSON(const nlohmann::json &json) { (void)json; }

nlohmann::json Program::toJSON() {
  using namespace nlohmann;

  json program;
  json arguments = this->arguments->toJSON();

  program["arguments"] = arguments;

  if (this->ticker != nullptr) {
    program["Ticker"] = this->ticker->toJSON();
  }

  return program;
}
