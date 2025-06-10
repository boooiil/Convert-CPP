// EnumToStringFactory.cpp
#include "EnumToStringFactory.h"

std::unordered_map<std::type_index, EnumToStringRegistryBase*> EnumToStringFactory::registries;
