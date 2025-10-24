#ifndef CONTAINER_FACTORY_H
#define CONTAINER_FACTORY_H

#include "BaseContainer.h"

#include <functional>
#include <string>
#include <unordered_map>

class ContainerFactory {
public:
  static auto create(const std::string &name) -> BaseContainer *;

private:
  static auto initialize() -> void;
  static std::unordered_map<std::string, std::function<BaseContainer *()>>
      codec_map;
};

#endif // CONTAINER_FACTORY_H
