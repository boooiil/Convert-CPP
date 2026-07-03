#include "ArgumentRegistry.h"
#include "Registry.h"
#include "src/program/settings/enums/Command_N.h"
#include "src/utils/ListUtils.h"
#include "src/utils/StringUtils.h"

#include <nlohmann/json.hpp>
#include <src/utils/logging/Logger.h>
#include <string>

void ArgumentRegistry::parse(const std::vector<std::string> &args) {
  if (args.size() < 2) {
    LOG_DEBUG(Logger::Priority::INFO,
              "No arguments supplied to the program options.");
    return;
  }

  LOG_DEBUG(Logger::Priority::INFO,
            "Parsing supplied arguments: " + ListUtils::join(args, ", "));

  Registry<std::string, Command_N::Command> &flagMap = getFlagMap();

  // skip the first argument (the program name)
  for (u_long i = 1; i < args.size(); i++) {
    LOG_DEBUG(Logger::Priority::INFO, "Parsing argument:", args[i]);

    // get the lowercase version of the argument
    std::string option = StringUtils::toLowerCase(args[i]);

    if (!flagMap.has(option)) {
      LOG_DEBUG(Logger::Priority::INFO,
                "Tried to parse an argument that was not registered: " +
                    option);
      continue;
    }

    Command_N::Command &command = flagMap.get(option);

    // check if the argument has been registered
    GenericArgument &argument = Registry::get(command);

    if (argument.isFlag()) {
      argument.parse("true");
      continue;
    }

    argument.parse(args[++i]);

    // if the supplied parameter is not valid, print an error
    if (argument.isErrored()) {
      invalidArgument(std::string(args[i - 1]) +
                      " was provided invalid parameter " +
                      std::string(args[i]));
      LOG(Logger::Priority::INFO, argument.getHelpMessage());
      continue;
    }
  }
}

void ArgumentRegistry::invalidArgument(const std::string &arg) {
  LOG_DEBUG(Logger::Priority::INFO, "Invalid argument:", arg);
}

void ArgumentRegistry::fromJSON(const nlohmann::json &json) {
  // Implementation for deserializing from JSON
  (void)json;
}

nlohmann::json ArgumentRegistry::toJSON(void) {
  nlohmann::json json;

  for (const auto &argument : values()) {
    // check if flag exists in arguments
    // skip --

    json[argument.get().getFlag()] = argument.get().toString();
  }

  return json;
}