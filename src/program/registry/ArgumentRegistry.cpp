#include "ArgumentRegistry.h"
#include "Registry.h"
#include "src/program/settings/arguments/FlagArgument.h"
#include "src/program/settings/enums/Command_N.h"
#include "src/utils/ListUtils.h"
#include "src/utils/StringUtils.h"

#include <nlohmann/json.hpp>
#include <src/utils/logging/Logger.h>
#include <string>

void ArgumentRegistry::parse(std::vector<std::string> args) {
  if (args.size() < 2) {
    LOG_DEBUG("No arguments supplied to the program options.");
  }

  LOG_DEBUG("Parsing supplied arguments: " + ListUtils::join(args, ", "));

  Registry<std::string, Command_N::Command> flag_registry = flagRegistry();

  // skip the first argument (the program name)
  for (int i = 1; i < args.size(); i++) {
    LOG_DEBUG("Parsing argument:", args[i]);

    // get the lowercase version of the argument
    std::string option = StringUtils::toLowerCase(args[i]);

    if (!flag_registry.has(option)) {
      LOG_DEBUG("Tried to parse an argument that was not registered: " +
                option);
      continue;
    }

    Command_N::Command *command = flag_registry.get(option);

    // check if the argument has been registered
    GenericArgument *argument = this->get(*command);

    // parse argument as a flag argument
    FlagArgument *flagArgument = dynamic_cast<FlagArgument *>(argument);

    // if the argument is a flag argument, parse it as such
    // we do this since we do not need to check the next argument
    // for a parameter, we just set it to true
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

Registry<std::string, Command_N::Command> ArgumentRegistry::flagRegistry(void) {
  Registry<std::string, Command_N::Command> result;

  for (const auto &argument : keySet()) {
    GenericArgument *arg = get(argument);

    result.add(arg->getFlag(), std::make_unique<Command_N::Command>(argument));
    result.add(arg->getLongFlag(),
               std::make_unique<Command_N::Command>(argument));
  }

  return result;
}

void ArgumentRegistry::invalidArgument(std::string arg) {
  LOG_DEBUG("Invalid argument:", arg);
}

void ArgumentRegistry::fromJSON(const nlohmann::json json) {
  // Implementation for deserializing from JSON
  (void)json;
}

nlohmann::json ArgumentRegistry::toJSON(void) {
  nlohmann::json json;

  for (const auto &argument : values()) {
    // check if flag exists in arguments
    // skip --

    json[argument->getFlag()] = argument->toString();
  }

  return json;
}