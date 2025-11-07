#ifndef ENUM_TO_STRING_REGISTRY_H
#define ENUM_TO_STRING_REGISTRY_H

#include "EnumToStringRegistryBase.h"
#include "Registry.h"
#include <stdexcept>
#include <typeindex>

// stored as    map<int, RegistryLike>
// RegistryLike map<T, StringEnumDataHolder>
class EnumToStringRegistryMaster
    : public Registry<std::type_index, EnumToStringRegistryBase> {
public:
  EnumToStringRegistryMaster() = default;

  template <typename K, typename V>
  EnumToStringRegistryBase *getOrThrow(std::type_index id) {
    auto it = this->registry.find(id);
    if (it == this->registry.end()) {
      throw std::runtime_error("Registry not found for id " +
                               std::string(id.name()));
    }

    auto *casted =
        dynamic_cast<Registry<K, StringEnumDataHolder> *>(it->second.get());
    if (!casted)
      throw std::runtime_error("Registry type mismatch for id " +
                               std::string(id.name()));
    return casted;
  }

private:
};

#endif // !ENUM_TO_STRING_REGISTRY_H
