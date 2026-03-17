#ifndef A1F7CE3A_F061_4B17_BFE5_1E7809093845
#define A1F7CE3A_F061_4B17_BFE5_1E7809093845

#include "Definition.h"
#include "src/program/registry/Registry.h"
#include <string>

class DefinitionRegistry {
public:
  static auto get(const std::string &name) -> const Definition & {
    if (definition_registry.empty()) {
      initialize();
    }

    auto result = definition_registry.get(name);
    if (result != nullptr) {
      return *result;
    }

    return *(new Definition(
        name, {}, "Unimplemented Definition",
        "The definition requested does not have an implementation."));
  }

  static auto registry() -> const Registry<std::string, const Definition> & {
    if (definition_registry.empty()) {
      initialize();
    }

    return definition_registry;
  }

  template <typename T>
    requires std::is_enum_v<T>
  static auto enumFromDefName(const std::string &name) -> T {
    if (definition_registry.empty()) {
      initialize();
    }

    const Definition &def = get(name);
    return def.getEnumAssoc<T>();
  }

  template <typename T>
    requires std::is_enum_v<T>
  static auto defFromEnum(const T &enumValue) -> const Definition & {
    if (definition_registry.empty()) {
      initialize();
    }

    for (const Definition *val : definition_registry.values()) {
      if (val->getEnumType() != typeid(T)) {
        continue;
      }

      if (val->getEnumAssoc<T>() == enumValue)
        return *val;
    }

    throw std::logic_error("No definition found for enum value." +
                           std::string(typeid(T).name()) + " value " +
                           std::to_string(static_cast<int>(enumValue)));
    return get("invalid");
  }

private:
  static auto initialize() -> void;
  static Registry<std::string, const Definition> definition_registry;
};

#endif /* A1F7CE3A_F061_4B17_BFE5_1E7809093845 */
