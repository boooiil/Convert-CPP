#include "../../../utils/logging/LogColor.h"
#include "../../../utils/logging/Logger.h"
#include "../../../utils/StringUtils.h"
#include "../../Program.h"
#include "../arguments/FlagArgument.h"
#include "../arguments/IntegerArgument.h"
#include "ParentOptions.h"

ParentOptions::ParentOptions(void) {
  this->arguments = std::make_unique<ArgumentRegistry>();
}

ParentOptions::~ParentOptions(void) {
  // Destructor
}

void ParentOptions::prepare(void) {
  // Prepare the options

  std::shared_ptr<IntegerArgument> ia = std::make_shared<IntegerArgument>(
    "-a", "--amount", "Amount of media to process", 1);

  this->arguments->add(Command::AMOUNT, ia);

  ia = std::make_shared<IntegerArgument>(
    "-dr", "--displayrefresh",
    "Set the display refresh rate for the output. (Will default to the "
    "original refresh rate if not specified)",
    1000);

  this->arguments->add(Command::DISPLAYREFRESH, ia);

  std::shared_ptr<FlagArgument> fa = std::make_shared<FlagArgument>("-h", "--help", "Print the help message.",
    false);

  this->arguments->add(Command::HELP, fa);

  fa = std::make_shared<FlagArgument>(
    "-hwd", "--hardwaredecode", "Use hardware decoding if available.", true);

  this->arguments->add(Command::HARDWAREDECODE, fa);

  fa = std::make_shared<FlagArgument>(
    "-hwe", "--hardwareencode", "Use hardware encoding if available.", false);

  this->arguments->add(Command::HARDWAREENCODE, fa);

  fa = std::make_shared<FlagArgument>(
    "-i", "--info", "Print information about the input file.", false);

  this->arguments->add(Command::INFO, fa);
}

void ParentOptions::parse(std::vector<std::string>& args) {

  this->i_args = args;

  if (args.size() < 2) {
    LOG_DEBUG("No arguments supplied to the parent options.");
  }

  // skip the first argument (the program name)
  for (int i = 1; i < args.size(); i++) {

    LOG("Parsing argument: ", args[i]);

    // get the lowercase version of the argument
    std::string option = StringUtils::toLowerCase(args[i]);

    // check if the argument has been registered
    std::shared_ptr<GenericArgument> argument = this->arguments->get(option);

    // if the argument is not registered, print an error and continue
    if (argument == nullptr) {
      invalidArgument(args[i]);
      continue;
    }

    // parse argument as a flag argument
    std::shared_ptr<FlagArgument> flagArgument =
      std::dynamic_pointer_cast<FlagArgument>(argument);

    // if the argument is a flag argument, parse it as such
    if (flagArgument != nullptr) {
      flagArgument->parse("true");
      continue;
    }
    // if the argument is not a flag argument, it must be a complex argument
    else {

      // check if the next argument is a parameter
      argument->parse(args[++i]);

      // if the supplied parameter is not valid, print an error
      if (argument->isErrored()) {
        invalidArgument(std::string(args[i - 1]) +
          " was provided invalid parameter " +
          std::string(args[i]));
        LOG(argument->getHelpMessage());
        continue;
      }
    }
  }
}

void ParentOptions::validate(void) {
  // Validate the options
}

void ParentOptions::invalidArgument(std::string arg) {
  LOG(LogColor::fgRed(arg));
  Program::stopFlag = true;
}

void ParentOptions::fromJSON(const nlohmann::json json) {

}

nlohmann::json ParentOptions::toJSON(void) {
  nlohmann::json json;

  json["arguments"] = this->arguments->toJSON();
  json["i_args"] = this->i_args;

  return json;
}




