#ifndef B0A0C886_E2F5_4FB4_AD1D_59E799BF7601
#define B0A0C886_E2F5_4FB4_AD1D_59E799BF7601

#include "Registry.h"
#include "src/program/settings/arguments/GenericArgument.h"
#include "src/program/settings/enums/Command.h"

class ArgumentRegistry : public Registry<Command, GenericArgument> {
public:
  ~ArgumentRegistry(void) = default;

  Registry<std::string, Command> flagRegistry(void);

  void parse(std::vector<std::string> args);
  void invalidArgument(std::string arg);

  template <typename T> T *get_t(Command flag) {
    return dynamic_cast<T *>(this->get(flag));
  }

  void fromJSON(const nlohmann::json) override;
  nlohmann::json toJSON(void) override;

private:
};

#endif /* B0A0C886_E2F5_4FB4_AD1D_59E799BF7601 */
