#ifndef A338F53C_EB7A_48A9_B80E_9A4487AEF6AA
#define A338F53C_EB7A_48A9_B80E_9A4487AEF6AA

#include "EnumToStringRegistryBase.h"
#include "Registry.h"
#include "src/program/settings/enums/StringEnumDataHolder.h"
#include <memory>

template <typename T>
concept EnumType = std::is_enum_v<T>;

template <EnumType T>
class EnumToStringRegistry : public Registry<T, StringEnumDataHolder>,
                             public EnumToStringRegistryBase {
public:
  void _add(int key, std::string enumName, std::string enumProperName,
            std::string description) override {
    Registry<T, StringEnumDataHolder>::add(
        static_cast<T>(key), std::make_unique<StringEnumDataHolder>(
                                 enumName, enumProperName, description));
  }

  const StringEnumDataHolder *_get(int key) const override {
    return Registry<T, StringEnumDataHolder>::get(static_cast<T>(key));
  }

  const T fromNameTyped(const std::string &name) const {

    for (const auto &[key, val] : this->registry) {
      if (val->getName() == name)
        return key;
    }

    return T::INVALID;
  }
  int fromName(const std::string &name) const override {
    return static_cast<int>(fromNameTyped(name));
  }
};
#endif /* A338F53C_EB7A_48A9_B80E_9A4487AEF6AA */
