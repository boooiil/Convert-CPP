#ifndef FFMPEG_ARGUMENT_BUILDER_H
#define FFMPEG_ARGUMENT_BUILDER_H

#include <uuid.h>

#include <string>
#include <vector>

#include "audio/BaseAudioCodec.h"
#include "container/BaseContainer.h"
#include "src/program/registry/ArgumentRegistry.h"
#include "src/program/settings/arguments/VectorArgument.h"

/* Builder created -> builder.build() */

class Media;

class FFmpegArgumentBuilder {
public:
  FFmpegArgumentBuilder(Media *media);
  ~FFmpegArgumentBuilder();

  void assertAudio(VectorArgument<int> &);
  void assertSubtitle(VectorArgument<int> &);

  std::vector<BaseAudioCodec *> generateAudioCodecs(VectorArgument<int> &,
                                                    ArgumentRegistry &);
  std::vector<BaseSubtitleCodec *> generateSubtitleCodecs(VectorArgument<int> &,
                                                          ArgumentRegistry &);

  // if container supports video codec, use
  // else stop program
  // others use default
  void validate();
  std::vector<std::string> build();

private:
  BaseContainer *container = nullptr;
  Media *media = nullptr;
};

#endif // FFMPEG_ARGUMENT_BUILDER_H
