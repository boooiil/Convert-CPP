#ifndef FFMPEG_ARGUMENT_BUILDER_H
#define FFMPEG_ARGUMENT_BUILDER_H

#include <string>
#include <vector>

#include "container/BaseContainer.h"
#include <stduuid/uuid.h>

/* Builder created -> builder.build() */

class FFmpegArgumentBuilder {
public:
  FFmpegArgumentBuilder(BaseContainer* _container);
  ~FFmpegArgumentBuilder();

  // if container supports video codec, use
  // else stop program
  // others use default
  void validate(uuids::uuid id);
  std::vector<std::string> build();

private:
  BaseContainer* container;
};

#endif  // FFMPEG_ARGUMENT_BUILDER_H