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

  template <typename T, typename U>
  void assertStreamMapping(const VectorArgument<T> &streams,
                           std::vector<U> &existingStreamMap) {

    // // wanted index total
    // int w_index_total = 0;
    // // exist index total
    // int e_index_total = 0;

    // for (u_long i = 0; i < streams.get().size(); i++) {
    //   LOG_DEBUG(Logger::Priority::INFO,"WANTED STREAM INDEX: ",
    //   streams.get()[i]); w_index_total += streams.get()[i];
    // }

    // for (u_long i = 0; i < existingStreamMap.size(); i++) {
    //   LOG_DEBUG(Logger::Priority::INFO,"EXISTING STREAM INDEX: ", i);
    //   e_index_total += i;
    // }

    // LOG_DEBUG(Logger::Priority::INFO,"W:", w_index_total, "E:",
    // e_index_total);

    // need to assert that the audio codecs
    //

    if (streams.get().size() > existingStreamMap.size()) {
      LOG_DEBUG(Logger::Priority::INFO,
                "WANTED INDEX TOTAL IS GREATER THAN EXISTING INDEX TOTAL");
      LOG_DEBUG(Logger::Priority::INFO,
                "WANTED INDEX TOTAL: ", streams.get().size());
      LOG_DEBUG(Logger::Priority::INFO,
                "EXISTING INDEX TOTAL: ", existingStreamMap.size());
      LOG(Logger::Priority::INFO,
          LogColor::fgRed("Wanted indexes exceed existing streams for: " +
                          std::string(typeid(T).name()) + " and " +
                          std::string(typeid(U).name())));
      term();
      // throw std::invalid_argument(
      //     "Wanted audio indexes exceed existing audio streams.");
    }
  };
  void assertSubtitle(const VectorArgument<int> &);

  std::vector<BaseAudioCodec *> generateAudioCodecs(const VectorArgument<int> &,
                                                    ArgumentRegistry &);
  std::vector<BaseSubtitleCodec *>
  generateSubtitleCodecs(const VectorArgument<int> &, ArgumentRegistry &);
  std::vector<BaseAttachment *> generateAttachments();

  // if container supports video codec, use
  // else stop program
  // others use default
  void validate();
  std::vector<std::string> build();

private:
  BaseContainer *container = nullptr;
  Media *media = nullptr;

  void term();
};

#endif // FFMPEG_ARGUMENT_BUILDER_H
