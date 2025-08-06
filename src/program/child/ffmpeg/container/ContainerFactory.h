#ifndef CONTAINER_FACTORY_H
#define CONTAINER_FACTORY_H

#include "../../../settings/enums/Container.h"
#include "BaseContainer.h"
#include "Container_MKV.h"
#include "Container_MP4.h"

class ContainerFactory {
 public:
  static BaseContainer* createContainer(Container containerType) {
    switch (containerType) {
      case Container::MKV:
        return new Container_MKV();
      case Container::MP4:
        return new Container_MP4();
        //   case Container::WEBM:
        //     return new Container_WEBM();
        //   case Container::AVI:
        //     return new Container_AVI();
        //   case Container::MOV:
        //     return new Container_MOV();
      default:
        throw std::runtime_error("Unsupported container type");
    }
  }
  static BaseContainer* createContainer(const std::string& containerName) {
    if (containerName == "mkv") {
      return new Container_MKV();
    } else if (containerName == "mp4") {
      return new Container_MP4();
      // } else if (containerName == "webm") {
      //   return new Container_WEBM();
      // } else if (containerName == "avi") {
      //   return new Container_AVI();
      // } else if (containerName == "mov") {
      //   return new Container_MOV();
    }
    throw std::runtime_error("Unsupported container type: " + containerName);
  }
};

#endif  // CONTAINER_FACTORY_H