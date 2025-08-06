#include "ArgumentRegistry.h"

#include <nlohmann/json.hpp>
#include <stdexcept>
#include <string>
#include <unordered_map>

#include "../../../utils/ListUtils.h"
#include "../../../utils/StringUtils.h"
#include "../../../utils/logging/Logger.h"
#include "FlagArgument.h"
#include "GenericArgument.h"

// std::unordered_map<Command, GenericArgument*> ArgumentRegistry::arguments =
// {};

std::unordered_map<std::string, Command> ArgumentRegistry::flag_to_command_map =
    {{"-a", Command::AMOUNT},
     {"--amount", Command::AMOUNT},
     {"-ac", Command::AUDIOCHANNELS},
     {"--audiochannels", Command::AUDIOCHANNELS},
     {"-aco", Command::AUDIOCODEC},
     {"--audiocodec", Command::AUDIOCODEC},
     {"-as", Command::AUDIOSTREAMS},
     {"--audiostreams", Command::AUDIOSTREAMS},
     {"-b", Command::BITRATE},
     {"--bitrate", Command::BITRATE},
     {"-c", Command::CROP},
     {"--crop", Command::CROP},
     {"-co", Command::CONSTRAIN},
     {"--constrain", Command::CONSTRAIN},
     {"-con", Command::CONTAINER},
     {"--container", Command::CONTAINER},
     {"-crf", Command::CRF},
     {"--crf", Command::CRF},
     {"-dr", Command::DISPLAYREFRESH},
     {"--displayrefresh", Command::DISPLAYREFRESH},
     {"-e", Command::ENCODER},
     {"--encoder", Command::ENCODER},
     {"-h", Command::HELP},
     {"--help", Command::HELP},
     {"-hwd", Command::HARDWAREDECODE},
     {"--hardwaredecode", Command::HARDWAREDECODE},
     {"-hwe", Command::HARDWAREENCODE},
     {"--hardwareencode", Command::HARDWAREENCODE},
     {"-i", Command::INFO},
     {"--info", Command::INFO},
     {"-lf", Command::LOGGINGOPTIONS},
     {"--loggingoptions", Command::LOGGINGOPTIONS},
     {"-o", Command::OVERWRITE},
     {"--overwrite", Command::OVERWRITE},
     {"-p", Command::PARENT},
     {"--parent", Command::PARENT},
     {"-ss", Command::START},
     {"--start", Command::START},
     {"-tr", Command::TRIM},
     {"--trim", Command::TRIM}};

ArgumentRegistry::~ArgumentRegistry(void) {
  LOG_DEBUG("Destroying ArgumentRegistry...");
  LOG_DEBUG("Expecting to destroy { GenericArgument }");
  for (auto &[command, argument] : this->arguments) {
    if (argument != nullptr) {
      delete argument;
    }
  }
}

void ArgumentRegistry::add(std::string flag, GenericArgument *argument) {
  if (flag_to_command_map.find(flag) == flag_to_command_map.end()) {
    throw std::invalid_argument("Tried to add flag: '" + flag +
                                "'that does not exist.");
    return;
  }

  arguments[flag_to_command_map[flag]] = argument;
}

void ArgumentRegistry::add(Command flag, GenericArgument *argument) {
  arguments[flag] = argument;
}

void ArgumentRegistry::remove(std::string flag) {
  if (flag_to_command_map.find(flag) == flag_to_command_map.end()) {
    throw std::invalid_argument("Tried to remove flag: '" + flag +
                                "'that does not exist.");
    return;
  }

  arguments.erase(flag_to_command_map[flag]);
}

void ArgumentRegistry::remove(Command flag) { arguments.erase(flag); }

void ArgumentRegistry::update(std::string flag, GenericArgument *argument) {
  if (flag_to_command_map.find(flag) == flag_to_command_map.end()) {
    throw std::invalid_argument("Tried to update flag: '" + flag +
                                "'that does not exist.");
    return;
  }

  arguments[flag_to_command_map[flag]] = argument;
}

void ArgumentRegistry::update(Command flag, GenericArgument *argument) {
  arguments[flag] = argument;
}

void ArgumentRegistry::parse(std::vector<std::string> args) {
  if (args.size() < 2) {
    LOG_DEBUG("No arguments supplied to the program options.");
  }

  LOG_DEBUG("Parsing supplied arguments: " + ListUtils::join(args, ", "));

  // skip the first argument (the program name)
  for (int i = 1; i < args.size(); i++) {
    LOG_DEBUG("Parsing argument:", args[i]);

    // get the lowercase version of the argument
    std::string option = StringUtils::toLowerCase(args[i]);

    if (!this->has(option)) {
      LOG_DEBUG("Tried to parse an argument that was not registered: " +
                option);
      continue;
    }

    // check if the argument has been registered
    GenericArgument *argument = this->get(option);

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

void ArgumentRegistry::invalidArgument(std::string arg) {
  LOG_DEBUG("Invalid argument:", arg);
}

GenericArgument *ArgumentRegistry::get(std::string flag) {
  // iterate over the flag and long flag of the arguments and return the
  // argument that matches the flag

  if (flag_to_command_map.find(flag) == flag_to_command_map.end()) {
    LOG_DEBUG("Tried to get flag: '" + flag + "' that does not exist.");
    // std::invalid_argument("Tried to get flag: '" + flag + "'that does not
    // exist.");
    return nullptr;
  }

  return arguments[flag_to_command_map[flag]];
}

GenericArgument *ArgumentRegistry::get(Command flag) { return arguments[flag]; }

const std::unordered_map<std::string, Command>
ArgumentRegistry::get_flag_to_command_map() const {
  return flag_to_command_map;
}

const std::unordered_map<Command, GenericArgument *>
ArgumentRegistry::get_all() {
  return arguments;
}

bool ArgumentRegistry::has(std::string flag) const {
  if (!flag_to_command_map.contains(flag)) {
    // std::invalid_argument("Tried to check if flag: '" + flag +
    // "' exists, but it does not.");
    return false;
  }

  Command command = flag_to_command_map.at(flag);

  return arguments.contains(command);
}

bool ArgumentRegistry::has(Command flag) const {
  return arguments.contains(flag);
}

void ArgumentRegistry::fromJSON(const nlohmann::json json) {
  // Implementation for deserializing from JSON
  (void)json;
}

nlohmann::json ArgumentRegistry::toJSON(void) {
  nlohmann::json json;

  for (const auto &[command, argument] : arguments) {
    // check if flag exists in arguments
    // skip --

    json[argument->getFlag()] = argument->toString();
  }

  return json;
}