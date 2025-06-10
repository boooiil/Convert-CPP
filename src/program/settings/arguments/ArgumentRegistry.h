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
  void add(std::string flag, std::shared_ptr<GenericArgument> argument);
  void add(Command, std::shared_ptr<GenericArgument> argument);
  void remove(std::string flag);
  void remove(Command flag);
  void update(std::string flag, std::shared_ptr<GenericArgument> argument);
  void update(Command flag, std::shared_ptr<GenericArgument> argument);
  std::shared_ptr<GenericArgument> get(std::string flag);
  std::shared_ptr<GenericArgument> get(Command flag);

  const std::unordered_map<Command, std::shared_ptr<GenericArgument>>
    get_all();

  const std::unordered_map<std::string, Command> get_flag_to_command_map() const;

  template <typename T>
  static std::shared_ptr<T> get_t(std::string flag) {

    if (flag_to_command_map.find(flag) ==
      flag_to_command_map.end()) {
      throw std::invalid_argument("Tried to get flag: '" + flag + "'that does not exist.");
      return nullptr;
    }

    //Command command = flag_to_command_map[flag];

    return std::dynamic_pointer_cast<T>(arguments[flag_to_command_map[flag]]);
  }

  template <typename T>
  static std::shared_ptr<T> get_t(Command flag) {
    return std::dynamic_pointer_cast<T>(arguments[flag]);
  }

  /*using addFn = void (*)(std::string, std::shared_ptr<GenericArgument>);
  using addCommandFn = void (*)(Command, std::shared_ptr<GenericArgument>);
  using removeFn = void (*)(std::string);
  using removeCommandFn = void (*)(Command);
  using updateFn = void (*)(std::string, std::shared_ptr<GenericArgument>);
  using updateCommandFn = void (*)(Command, std::shared_ptr<GenericArgument>);
  using getFn = std::shared_ptr<GenericArgument>(*)(std::string);
  using getCommandFn = std::shared_ptr<GenericArgument>(*)(Command);
  using getAllFn = const std::unordered_map<
    std::string, std::shared_ptr<GenericArgument>> (*)();
  using getAllCommandFn = const std::unordered_map <
    Command, std::shared_ptr<GenericArgument>> (*)();

  template <typename T>
  using getTFn = std::shared_ptr<T>(*)(std::string);

  template <typename T>
  using getCommandTFn = std::shared_ptr<T>(*)(Command);*/

  void fromJSON(const nlohmann::json json) override;
  nlohmann::json toJSON(void);

private:
  static std::unordered_map<Command, std::shared_ptr<GenericArgument>>
    arguments;
  static std::unordered_map<std::string, Command> flag_to_command_map;
};

#endif  // !ARGUMENT_REGISTRY_H
