#ifndef FFMPEG_ARGUMENT_BUILDER_H
#define FFMPEG_ARGUMENT_BUILDER_H

#include <uuid.h>

#include <string>
#include <vector>

#include "container/BaseContainer.h"

/* Builder created -> builder.build() */

class Media;

class FFmpegArgumentBuilder {
public:
  FFmpegArgumentBuilder(Media *media);
  ~FFmpegArgumentBuilder();

  // if container supports video codec, use
  // else stop program
  // others use default
  void validate(uuids::uuid id);
  std::vector<std::string> build();

private:
  BaseContainer *container;
  Media *media;
};

#endif // FFMPEG_ARGUMENT_BUILDER_H