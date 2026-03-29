#ifndef ATTACHMENT_FACTORY_H
#define ATTACHMENT_FACTORY_H

#include "BaseAttachment.h"
#include "src/program/registry/Registry.h"
#include <functional>
#include <string>

class AttachmentFactory {
public:
  static auto create(const std::string &name) -> BaseAttachment *;
  static auto registry()
      -> const Registry<std::string, std::function<BaseAttachment *()>> &;

private:
  static auto initialize() -> void;
  static Registry<std::string, std::function<BaseAttachment *()>>
      codec_registry;
};

#endif // ATTACHMENT_FACTORY_H
