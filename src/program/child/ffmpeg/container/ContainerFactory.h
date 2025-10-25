#ifndef CONTAINER_FACTORY_H
#define CONTAINER_FACTORY_H

#include "BaseContainer.h"
#include "src/program/registry/Registry.h"

#include <functional>
#include <string>

class ContainerFactory {
public:
  static auto create(const std::string &name) -> BaseContainer *;
  static auto registry()
      -> const Registry<std::string, std::function<BaseContainer *()>> &;

private:
  static auto initialize() -> void;
  static Registry<std::string, std::function<BaseContainer *()>> codec_registry;
};

#endif // CONTAINER_FACTORY_H
