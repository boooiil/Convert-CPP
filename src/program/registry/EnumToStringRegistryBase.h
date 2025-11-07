#ifndef ENUM_TO_STRING_REGISTRY_BASE_H
#define ENUM_TO_STRING_REGISTRY_BASE_H

#include "src/program/settings/enums/StringEnumDataHolder.h"

class EnumToStringRegistryBase {
public:
  virtual ~EnumToStringRegistryBase() = default;

  virtual void _add(int key, std::string enumName, std::string enumProperName,
                    std::string description) = 0;
  virtual const StringEnumDataHolder *_get(int) const = 0;
  virtual int fromName(const std::string &name) const = 0;
};

#endif // !ENUM_TO_STRING_REGISTRY_BASE_H
