#ifndef ENUM_TO_STRING_REGISTRY_BASE_H
#define ENUM_TO_STRING_REGISTRY_BASE_H

#include "StringEnumDataHolder.h"

class EnumToStringRegistryBase {
public:
  virtual ~EnumToStringRegistryBase() = default;

  virtual void add(int key, std::string enumName, std::string enumProperName, std::string description) = 0;
  virtual const StringEnumDataHolder& get(int) const = 0;
  virtual const int fromName(const std::string& name) const = 0;
};

#endif // !ENUM_TO_STRING_REGISTRY_BASE_H
