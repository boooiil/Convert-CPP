#include "FFmpegArgumentBuilder.h"

#include "../../../program/Program.h"
#include "../../../utils/logging/Logger.h"
#include "../../settings/arguments/ArgumentRegistry.h"
#include "../../settings/arguments/VectorArgument.h"
#include "../../settings/enums/SubtitleCodec.h"
#include "container/BaseContainer.h"
#include <string>
#include <vector>

FFmpegArgumentBuilder::FFmpegArgumentBuilder(BaseContainer* _container)
  : container(_container) {
}

FFmpegArgumentBuilder::~FFmpegArgumentBuilder() {
  LOG_DEBUG("Deconstructing...");
  if (container != nullptr) {
    LOG_DEBUG("Deleting Container.");
    delete container;
  }
}

void FFmpegArgumentBuilder::validate(uuids::uuid id) {

  ChildOptions& childOptions = *Program::settings->childOptionsMap[id];
  ArgumentRegistry& argumentRegistry = *childOptions.argumentRegistry;

  // Running: Settings Init -> Container(a, v, s) -> Validate -> Init A, V, S ->
  // Adapted Container

  // Think for a second. Here are the multiple ways we could handle creating a
  // container:
  // Settings Init -> Container(settings) -> Adapted Container
  // Settings Init -> Container(a, v, s)  -> Validate Container -> Adapted
  // Container

  // TODO: store results of validation in the codec instances as well as
  // updating the program settings
  // if video codec is not supported, stop program
  // if audio codec is not supported, use default
  // if subtitle codec is not supported, use default

  // TODO: default behavior for incorrect validation should result
  // in a stop flag and a display of acceptable parameters. However,
  // I kinda don't want to do that as the defaults are generally okay
  // for me and I figure that most people would understand acceptable params
  // on their desired codecs.

  // validate with codec.getXXX() as the result modifies the default
  // TODO: rename to codec.validateXXX() for better clarity;
  // ie: this.audio_codec->getChannels(int) -> BaseAudioCodec.runningChannels =
  // getChannels():int or getFallbackChannel():int
  // this.audio_codec.getRunningChannel() -> BaseAudioCodec.runningChannels;

  // sample rate (current = default)
  // bit depth (current = default)
  auto wanted_encoder_codec = argumentRegistry.get_t<BaseArgument<Encoders>>(Command::ENCODER)->get();
  auto wanted_encoder_tune = argumentRegistry.get_t<BaseArgument<Tunes>>(Command::TUNE)->get();
  auto wanted_encoder_level = 4.1;
  auto wanted_encoder_preset = "slow";

  auto wanted_audio_codec = argumentRegistry.get_t<VectorArgument<std::string>>("-aco")->get();
  auto wanted_audio_channels = argumentRegistry.get_t<VectorArgument<int>>("-ac")->get();
  auto wanted_audio_bit_depth = 16;
  auto wanted_audio_sample_rate = 48000;

  auto wanted_subtitle_codec = SubtitleCodec::ASS;

  auto validated_encoder_codec =
    this->container->getVideoCodec(wanted_encoder_codec);

  if (validated_encoder_codec != wanted_encoder_codec) {
    LOG_DEBUG(
      "Wanted encoder was not in the acceptable encoders list for this "
      "container.",
      EnumToStringFactory::get(wanted_encoder_codec).getName());

    Program::stopFlag = true;
    return;
  }
}

std::vector<std::string> FFmpegArgumentBuilder::build() { return std::vector<std::string>(); }
