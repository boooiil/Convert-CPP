#ifndef ENUM_TO_STRING_REGISTRY_H
#define ENUM_TO_STRING_REGISTRY_H

#include <string>
#include <type_traits>
#include <unordered_map>

#include "EnumToStringRegistryBase.h"

template <typename T>
concept EnumType = std::is_enum_v<T>;

template <EnumType T>
class EnumToStringRegistry : public EnumToStringRegistryBase {
 public:
  void add(T key, std::string enumName, std::string enumProperName,
           std::string description) {
    this->registry[key] =
        StringEnumDataHolder(enumName, enumProperName, description);
  }
  void add(int key, std::string enumName, std::string enumProperName,
           std::string description) override {
    add(static_cast<T>(key), std::move(enumName), std::move(enumProperName),
        std::move(description));
  }

  const StringEnumDataHolder& get(T key) const {
    StringEnumDataHolder unknown("unknown", "Unknown",
                                 "Unknown enum value passed.");

    auto reg_it = registry.find(key);

    return (reg_it != registry.end()) ? reg_it->second : unknown;
  }
  const StringEnumDataHolder& get(int key) const override {
    return get(static_cast<T>(key));
  }

  const T fromNameTyped(const std::string& name) const {
    for (const auto& [key, val] : registry) {
      if (val.getName() == name) return key;
    }

    return T::INVALID;
  }
  const int fromName(const std::string& name) const override {
    return static_cast<int>(fromNameTyped(name));
  }

 private:
  std::unordered_map<T, StringEnumDataHolder> registry = {};
};

#endif  // !ENUM_TO_STRING_REGISTRY_H
