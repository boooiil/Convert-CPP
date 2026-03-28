#ifndef B0A0C886_E2F5_4FB4_AD1D_59E799BF7601
#define B0A0C886_E2F5_4FB4_AD1D_59E799BF7601

#include "Registry.h"
#include "src/program/settings/arguments/GenericArgument.h"
#include "src/program/settings/enums/Command_N.h"

class ArgumentRegistry : public Registry<Command_N::Command, GenericArgument> {
public:
  ~ArgumentRegistry(void) = default;

  // template <typename T> T &get(Command_N::Command flag) {
  //   return dynamic_cast<T &>(Registry::get(flag));
  // }

  template <Command_N::Command C> auto &get() {
    using T = typename CommandValueType<C>::type;
    return dynamic_cast<T &>(Registry::get(C));
  }

  ArgumentRegistry &add(const Command_N::Command &key,
                        std::unique_ptr<GenericArgument> value) override {
    commandRegistry.add(value->getFlag(),
                        std::make_unique<Command_N::Command>(key));
    if (!value->getLongFlag().empty()) {
      commandRegistry.add(value->getLongFlag(),
                          std::make_unique<Command_N::Command>(key));
    }
    Registry::add(key, std::move(value));
    return *this;
  }

  ArgumentRegistry &remove(const Command_N::Command &key) override {
    GenericArgument &argument = Registry::get(key);
    commandRegistry.remove(argument.getFlag());

    if (commandRegistry.has(argument.getLongFlag())) {
      commandRegistry.remove(argument.getLongFlag());
    }

    Registry::remove(key);
    return *this;
  }

  /**
   * @brief Get a registry of Commands mapped by argument flag.
   *
   * @return Registry<std::string, Command_N::Command>
   */
  Registry<std::string, Command_N::Command> &getFlagMap(void) {
    return commandRegistry;
  };

  void parse(const std::vector<std::string> &args);
  void invalidArgument(const std::string &arg);
  void fromJSON(const nlohmann::json &json) override;
  nlohmann::json toJSON(void) override;

private:
  Registry<std::string, Command_N::Command> commandRegistry =
      Registry<std::string, Command_N::Command>();
};

#endif /* B0A0C886_E2F5_4FB4_AD1D_59E799BF7601 */
