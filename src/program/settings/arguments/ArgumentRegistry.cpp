#include "ArgumentRegistry.h"

#include <memory>
#include <nlohmann/json.hpp>
#include <stdexcept>
#include <string>
#include <unordered_map>

#include "GenericArgument.h"

std::unordered_map<Command, std::shared_ptr<GenericArgument>>
ArgumentRegistry::arguments = {};

std::unordered_map<std::string, Command>
ArgumentRegistry::flag_to_command_map = {
    {"-a", Command::AMOUNT},
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
    {"-o", Command::OVERWRITE},
    {"--overwrite", Command::OVERWRITE},
    {"-p", Command::PARENT},
    {"--parent", Command::PARENT},
    {"-ss", Command::START},
    {"--start", Command::START},
    {"-tr", Command::TRIM},
    {"--trim", Command::TRIM}
};

void ArgumentRegistry::add(std::string flag,
  std::shared_ptr<GenericArgument> argument) {

  if (flag_to_command_map.find(flag) ==
    flag_to_command_map.end()) {
    throw std::invalid_argument("Tried to add flag: '" + flag + "'that does not exist.");
    return;
  }

  arguments[flag_to_command_map[flag]] = argument;
}

void ArgumentRegistry::add(Command flag,
  std::shared_ptr<GenericArgument> argument) {
  arguments[flag] = argument;
}

void ArgumentRegistry::remove(std::string flag) {

  if (flag_to_command_map.find(flag) ==
    flag_to_command_map.end()) {
    throw std::invalid_argument("Tried to remove flag: '" + flag + "'that does not exist.");
    return;
  }

  arguments.erase(flag_to_command_map[flag]);

}

void ArgumentRegistry::remove(Command flag) {
  arguments.erase(flag);
}

void ArgumentRegistry::update(std::string flag,
  std::shared_ptr<GenericArgument> argument) {

  if (flag_to_command_map.find(flag) ==
    flag_to_command_map.end()) {
    throw std::invalid_argument("Tried to update flag: '" + flag + "'that does not exist.");
    return;
  }

  arguments[flag_to_command_map[flag]] = argument;
}

void ArgumentRegistry::update(Command flag,
  std::shared_ptr<GenericArgument> argument) {
  arguments[flag] = argument;
}

std::shared_ptr<GenericArgument> ArgumentRegistry::get(std::string flag) {
  // iterate over the flag and long flag of the arguments and return the
  // argument that matches the flag

  if (flag_to_command_map.find(flag) ==
    flag_to_command_map.end()) {
    throw std::invalid_argument("Tried to get flag: '" + flag + "'that does not exist.");
    return nullptr;
  }

  return arguments[flag_to_command_map[flag]];
}

std::shared_ptr<GenericArgument> ArgumentRegistry::get(Command flag) {
  return arguments[flag];
}

const std::unordered_map<std::string, Command> ArgumentRegistry::get_flag_to_command_map() const {
  return flag_to_command_map;
}

const std::unordered_map<Command, std::shared_ptr<GenericArgument>>
ArgumentRegistry::get_all() {
  return arguments;
}

void ArgumentRegistry::fromJSON(const nlohmann::json json) {
  // Implementation for deserializing from JSON
}

nlohmann::json ArgumentRegistry::toJSON(void) {
  nlohmann::json json;

  for (const auto& [flag, command] : flag_to_command_map) {

    // check if flag exists in arguments
    // skip --

    if (flag.starts_with("--")) {
      continue; // skip long flags
    }

    if (arguments.find(command) == arguments.end()) {
      continue;
    }

    json[flag] = arguments[command]->toString();

  }

  return json;
}