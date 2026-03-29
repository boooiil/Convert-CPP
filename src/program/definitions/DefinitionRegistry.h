#ifndef A1F7CE3A_F061_4B17_BFE5_1E7809093845
#define A1F7CE3A_F061_4B17_BFE5_1E7809093845

#include "Definition.h"
#include "src/program/registry/Registry.h"
#include <functional>
#include <string>

class DefinitionRegistry {
public:
  static auto get(const std::string &name) -> const Definition & {
    if (definition_registry.empty()) {
      initialize();
    }

    // if not exist
    if (!definition_registry.has(name)) {
      return *(new Definition(
          name, {}, "Placeholder definition for " + name,
          "The definition requested does not have an implementation."));
    }

    return definition_registry.get(name);
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

    for (const std::reference_wrapper<const Definition> val :
         definition_registry.values()) {
      if (val.get().getEnumType() != typeid(T)) {
        continue;
      }

      if (val.get().getEnumAssoc<T>() == enumValue)
        return val.get();
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
