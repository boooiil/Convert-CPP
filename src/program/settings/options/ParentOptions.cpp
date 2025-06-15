#include "ParentOptions.h"

#include "../../../utils/StringUtils.h"
#include "../../../utils/logging/LogColor.h"
#include "../../../utils/logging/Logger.h"
#include "../../Program.h"
#include "../arguments/FlagArgument.h"
#include "../arguments/IntegerArgument.h"

ParentOptions::ParentOptions(void) : argumentRegistry(new ArgumentRegistry()) {}

ParentOptions::~ParentOptions(void) {
  LOG("Destroying ParentOptions...");
  LOG("Expecting to destroy { ArgumentRegistry }");
  if (this->argumentRegistry != nullptr) {
    delete this->argumentRegistry;
  }
  // Destructor
}

void ParentOptions::prepare(void) {
  // Prepare the options
  // amount

  argumentRegistry->add(
      Command::AMOUNT,
      new IntegerArgument("Amount of media to process", "-a", "--amount", 1));
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
    GenericArgument* argument = this->argumentRegistry->get(option);

    // if the argument is not registered, print an error and continue
    if (argument == nullptr) {
      invalidArgument(args[i]);
      continue;
    }

    // parse argument as a flag argument
    FlagArgument* flagArgument = (FlagArgument*)argument;

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
  LOG(LogColor::fgRed("Invalid argument" + arg));
  Program::stopFlag = true;
}

void ParentOptions::fromJSON(const nlohmann::json json) {}

nlohmann::json ParentOptions::toJSON(void) {
  nlohmann::json parentOptions;

  parentOptions["arguments"] = this->argumentRegistry->toJSON();
  parentOptions["i_args"] = this->i_args;

  return parentOptions;
}
