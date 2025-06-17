#ifndef ARGUMENT_REGISTRY_H
#define ARGUMENT_REGISTRY_H

#include <memory>
#include <stdexcept>
#include <string>
#include <unordered_map>

#include "../../generics/JSONSerializable.h"
#include "../enums/Command.h"
#include "../enums/StringEnumDataHolder.h"
#include "GenericArgument.h"

class ArgumentRegistry : public JSONSerializable {
 public:
  ~ArgumentRegistry(void);
  void add(std::string flag, GenericArgument* argument);
  void add(Command, GenericArgument* argument);
  void remove(std::string flag);
  void remove(Command flag);
  void update(std::string flag, GenericArgument* argument);
  void update(Command flag, GenericArgument* argument);
  void parse(std::vector<std::string> args);
  void invalidArgument(std::string arg);
  GenericArgument* get(std::string flag);
  GenericArgument* get(Command flag);

  const std::unordered_map<Command, GenericArgument*> get_all();

  const std::unordered_map<std::string, Command> get_flag_to_command_map()
      const;

  template <typename T>
  T* get_t(std::string flag) {
    if (flag_to_command_map.find(flag) == flag_to_command_map.end()) {
      throw std::invalid_argument("Tried to get flag: '" + flag +
                                  "'that does not exist.");
      return nullptr;
    }

    // Command command = flag_to_command_map[flag];

    return (T*)arguments[ArgumentRegistry::flag_to_command_map[flag]];
  }

  template <typename T>
  T* get_t(Command flag) {
    return (T*)arguments[flag];
  }

  /*using addFn = void (*)(std::string, GenericArgument*);
  using addCommandFn = void (*)(Command, GenericArgument*);
  using removeFn = void (*)(std::string);
  using removeCommandFn = void (*)(Command);
  using updateFn = void (*)(std::string, GenericArgument*);
  using updateCommandFn = void (*)(Command, GenericArgument*);
  using getFn = GenericArgument*(*)(std::string);
  using getCommandFn = GenericArgument*(*)(Command);
  using getAllFn = const std::unordered_map<
    std::string, GenericArgument*> (*)();
  using getAllCommandFn = const std::unordered_map <
    Command, GenericArgument*> (*)();

  template <typename T>
  using getTFn = T*(*)(std::string);

  template <typename T>
  using getCommandTFn = T*(*)(Command);*/
  bool has(std::string flag) const;
  bool has(Command flag) const;

  void fromJSON(const nlohmann::json json) override;
  nlohmann::json toJSON(void);

 private:
  std::unordered_map<Command, GenericArgument*> arguments;
  static std::unordered_map<std::string, Command> flag_to_command_map;
};

#endif  // !ARGUMENT_REGISTRY_H
