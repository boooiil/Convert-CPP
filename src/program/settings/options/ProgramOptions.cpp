#include <memory>
#include <nlohmann/json.hpp>
#include <regex>
#include <string>
#include <vector>

#include "../../../utils/logging/Logger.h"
#include "../arguments/FlagArgument.h"
#include "../arguments/IntegerArgument.h"
#include "ProgramOptions.h"
#include "src/program/settings/enums/HWAccelerators_N.h"
#include "src/program/settings/enums/LogFormat_N.h"
#include "src/program/settings/enums/Tunes_N.h"

// TODO: fill this out, rm ApplicationEncodingDecision

ProgramOptions::ProgramOptions() : argumentRegistry(new ArgumentRegistry()) {
  ProgramOptions::tuneRegex = {std::regex(R"(film)", std::regex::icase),
                               std::regex(R"(anim)", std::regex::icase),
                               std::regex(R"(grain)", std::regex::icase)};

  ProgramOptions::tuneAssociations = {Tunes_N::FILM, Tunes_N::ANIMATION,
                                      Tunes_N::GRAIN};
};

ProgramOptions::~ProgramOptions(void) {
  LOG_DEBUG("Destroying ProgramOptions...");
  LOG_DEBUG("Expecting to destroy { ArgumentRegistry }");
  if (this->argumentRegistry != nullptr) {
    delete this->argumentRegistry;
  }
};

void ProgramOptions::gatherSystemDetails(void) {}

void ProgramOptions::prepare(void) {
  /**
   * [x] logging
   * [x] parent
   * [x] refresh
   * [x] hwa
   * [x] hwd
   * [x] platform
   * [x] supported enc
   * [x] supported hwacc
   *
   */
  // logging, parent, refresh, hwa, hwd, hwe, platform, supported enc, supported
  // hwacc

  argumentRegistry->add(
      Command_N::DISPLAYREFRESH,
      std::make_unique<IntegerArgument>("Display refresh rate in ms", "-dr",
                                        "--displayrefresh", 1000));
  argumentRegistry->add(
      Command_N::HARDWAREACCEL,
      std::make_unique<EnumArgument<HWAccelerators_N::HWAccelerators>>(
          "Hardware accelerator to use", "-hwa", "--hwaccel",
          HWAccelerators_N::HWAccelerators::NONE));
  argumentRegistry->add(
      Command_N::HARDWAREDECODE,
      std::make_unique<FlagArgument>("Use hardware decoding if available",
                                     "-hwd", "--hardwaredecode", true));
  argumentRegistry->add(
      Command_N::HARDWAREENCODE,
      std::make_unique<FlagArgument>("Use hardware encoding if available",
                                     "-hwe", "--hardwareencode", false));
  argumentRegistry->add(Command_N::HELP,
                        std::make_unique<FlagArgument>("Print the help message",
                                                       "-h", "--help", false));
  argumentRegistry->add(
      Command_N::INFO,
      std::make_unique<FlagArgument>("Print information about the input file",
                                     "-i", "--info", false));
  argumentRegistry->add(Command_N::LOGGINGOPTIONS,
                        std::make_unique<EnumArgument<LogFormat_N::LogFormat>>(
                            "Logging format to use", "-lf", "--loggingformat",
                            LogFormat_N::DEFAULT));

  argumentRegistry->add(Command_N::PARENT, std::make_unique<FlagArgument>(
                                               "Run as parent process",
                                               "-parent", "--parent", false));
}

void ProgramOptions::parse(std::vector<std::string> args) {
  this->i_args = args;
  this->argumentRegistry->parse(args);
}

void ProgramOptions::validate(void) {}

void ProgramOptions::fromJSON(const nlohmann::json &json) {}

nlohmann::json ProgramOptions::toJSON() {
  nlohmann::json programSettings;

  programSettings["arguments"] = this->argumentRegistry->toJSON();

  programSettings["i_args"] = this->i_args;

  /*for (auto decoder : this->supportedDecoders) {
    programSettings["supported_decoders"].push_back(Decoders::getValue(decoder));
  }*/

  return programSettings;
}
