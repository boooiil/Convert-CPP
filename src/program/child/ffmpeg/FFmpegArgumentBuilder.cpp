#include "FFmpegArgumentBuilder.h"

#include <string>
#include <vector>

#include "../../../program/Program.h"
#include "../../../utils/logging/Logger.h"
#include "../../child/media/Media.h"
#include "../../settings/arguments/ArgumentRegistry.h"
#include "../../settings/arguments/EnumArgument.h"
#include "../../settings/arguments/FlagArgument.h"
#include "../../settings/arguments/StringArgument.h"
#include "../../settings/arguments/TimeStringVectorArgument.h"
#include "../../settings/arguments/VectorArgument.h"
#include "../../settings/arguments/video/Quality.h"
#include "audio/AudioCodecFactory.h"
#include "container/BaseContainer.h"
#include "container/ContainerFactory.h"
#include "video/VideoCodecFactory.h"

FFmpegArgumentBuilder::FFmpegArgumentBuilder(Media *_media) : media(_media) {
  ChildOptions &childOptions = *Program::settings->childOptionsMap[media->id];
  ArgumentRegistry &argumentRegistry = *childOptions.argumentRegistry;
  MediaFormat format = argumentRegistry.get_t<Quality>(Command::QUALITY)->get();

  Encoders wanted_enc =
      argumentRegistry.get_t<EnumArgument<Encoders>>(Command::ENCODER)->get();
  Container wanted_container =
      argumentRegistry.get_t<EnumArgument<Container>>(Command::CONTAINER)
          ->get();

  // audio streams desired by the user
  VectorArgument<int> *audioStreams =
      argumentRegistry.get_t<VectorArgument<int>>(Command::AUDIOSTREAMS);

  auto videoCodec = VideoCodecFactory::createVideoCodec(wanted_enc);
  auto audioCodecs = std::vector<BaseAudioCodec *>();
  this->container = ContainerFactory::createContainer(wanted_container);

#pragma region AUDIO SETTINGS

  // cases:
  // 1. audio streams (vec<int>) match audio codecs (vec<AudioCodec>)
  // - use provided codecs
  // 2. audio streams are greater than audio codecs
  // - use last codec?
  // - use existing codec?
  // 3. audio streams are less than audio codecs
  // - ignore
  // 4. audio streams are empty
  // - copy streams

  std::vector<std::string> wanted_codecs =
      argumentRegistry.get_t<VectorArgument<std::string>>(Command::AUDIOCODEC)
          ->get();
  std::vector<int> wanted_channels =
      argumentRegistry.get_t<VectorArgument<int>>(Command::AUDIOCHANNELS)
          ->get();

  for (int i = 0; i < media->probeResult->audioStreams.size(); i++) {
    BaseAudioCodec *audioCodec = nullptr;
    int media_channels = media->probeResult->audioStreams[i].channels;
    std::string media_audio_codec =
        media->probeResult->audioStreams[i].codec_name;

    // skip if stream index isnt in the
    // desired list
    if (!audioStreams->get().empty()) {
      // and if the audio stream is not in the list
      // skip the audio stream
      if (!ListUtils::contains(audioStreams->get(), i)) {
        LOG_DEBUG("Skipping audio stream", i, "for not being in the list.");
        continue;
      }
    }

    LOG_DEBUG("Audio stream", i, "is in the list.");

    /***************************************************
     *                                                 *
     *              AUDIO CODEC SETTINGS               *
     *                                                 *
     ***************************************************/

    // if there are more audio codecs than streams
    if (wanted_codecs.size() > i) {
      LOG_DEBUG("Audio index [", i, "] is using codec (", wanted_codecs[i],
                ")");
      audioCodec = AudioCodecFactory::createAudioCodec(wanted_codecs[i]);
    }
    // else use the last codec in the list
    else if (!wanted_codecs.empty()) {
      LOG_DEBUG("Audio index [", i, "] exceeded codecs, using last codec (",
                wanted_codecs[wanted_codecs.size() - 1], ")");
      audioCodec = AudioCodecFactory::createAudioCodec(
          wanted_codecs[wanted_codecs.size() - 1]);
    }
    // else copy the codec
    else {
      LOG_DEBUG("Audio index [", i, "] marked for copy codec.");
      audioCodec = AudioCodecFactory::createAudioCodec(media_audio_codec);
      // set bit depth
      audioCodec->setBitDepth(
          media->probeResult->audioStreams[i].bits_per_sample);
      // set sample rate
      audioCodec->setSampleRate(
          media->probeResult->audioStreams[i].sample_rate);
    }

    /***************************************************
     *                                                 *
     *            AUDIO CHANNEL SETTINGS               *
     *                                                 *
     ***************************************************/

    // if there are more audio channels than streams
    if (wanted_channels.size() > i) {
      LOG_DEBUG("Audio index [", i, "] is using channel (", wanted_channels[i],
                ")");
      audioCodec->setChannel(wanted_channels[i]);
    }
    // copy channels
    else {
      LOG_DEBUG("Audio index [", i, "] marked for copy channels.");
      audioCodec->setChannel(media_channels);
    }

    // set index
    audioCodec->setIndex(i);

    // push to audio codecs
    audioCodecs.push_back(audioCodec);
  }
#pragma endregion AUDIO SETTINGS

  this->container->setAudioCodec(audioCodecs);
  this->container->setVideoCodec(videoCodec);
}

FFmpegArgumentBuilder::~FFmpegArgumentBuilder() {
  LOG_DEBUG("Deconstructing...");
  if (container != nullptr) {
    LOG_DEBUG("Deleting Container.");
    delete container;
  }
}

void FFmpegArgumentBuilder::validate(uuids::uuid id) {
  // ChildOptions &childOptions = *Program::settings->childOptionsMap[id];
  // ArgumentRegistry &argumentRegistry = *childOptions.argumentRegistry;

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

  /**
   * 8/1/25
   * I should not have to use this validation function as the set methods within
   * the base classes throw on invalid parameters.
   *
   */

  // auto wanted_encoder_codec =
  //     argumentRegistry.get_t<BaseArgument<Encoders>>(Command::ENCODER)->get();
  // auto wanted_encoder_tune =
  //     argumentRegistry.get_t<BaseArgument<Tunes>>(Command::TUNE)->get();
  // auto wanted_encoder_level = 4.1;
  // auto wanted_encoder_preset = "slow";

  // auto wanted_audio_codec =
  //     argumentRegistry.get_t<VectorArgument<std::string>>(Command::AUDIOCODEC)
  //         ->get();
  // auto wanted_audio_channels =
  //     argumentRegistry.get_t<VectorArgument<int>>(Command::AUDIOCHANNELS)
  //         ->get();
  // auto wanted_audio_bit_depth = 16;
  // auto wanted_audio_sample_rate = 48000;

  // auto wanted_subtitle_codec = SubtitleCodec::ASS;

  // auto validated_encoder_codec =
  //     this->container->getVideoCodec(wanted_encoder_codec);

  // if (validated_encoder_codec != wanted_encoder_codec) {
  //   LOG_DEBUG(
  //       "Wanted encoder was not in the acceptable encoders list for this "
  //       "container.",
  //       EnumToStringFactory::get(wanted_encoder_codec).getName());

  //   Program::stopFlag = true;
  return;
}

std::vector<std::string> FFmpegArgumentBuilder::build() {
  // ProgramOptions& programSettings = *Program::settings->programOptions;
  ChildOptions &childOptions = *Program::settings->childOptionsMap[media->id];
  ArgumentRegistry &argumentRegistry = *childOptions.argumentRegistry;
  MediaFormat format = argumentRegistry.get_t<Quality>(Command::QUALITY)->get();

  std::vector<std::string> result;

  result.push_back("-v error -stats");

  if (argumentRegistry.get_t<FlagArgument>(Command::HARDWAREDECODE)->get()) {
    if (childOptions.runningHWAccel != HWAccelerators::INVALID) {
      result.push_back("-hwaccel " + EnumToStringFactory::get<HWAccelerators>(
                                         childOptions.runningHWAccel)
                                         .getName());
    }
  }

  result.push_back("-i \"" + media->file->originalFullPath + "\"");

  result.push_back("-map 0:v:0");

  VectorArgument<int> *audioStreams =
      argumentRegistry.get_t<VectorArgument<int>>(Command::AUDIOSTREAMS);

  // this is the new way since i added index to the
  // audio codecs.
  // all audio codecs should be mapped to the streams
  // regardless if the user specified a mapping.

#pragma region AUDIO ASSERTIONS

  // wanted index total
  int w_index_total = 0;
  // exist index total
  int e_index_total = 0;

  for (int i = 0; i < audioStreams->get().size(); i++) {
    LOG_DEBUG("WANTED AS INDEX: ", audioStreams->get()[i]);
    w_index_total += audioStreams->get()[i];
  }

  for (int i = 0; i < media->probeResult->audioStreams.size(); i++) {
    LOG_DEBUG("EXISTING AS INDEX: ", i);
    e_index_total += i;
  }

  LOG_DEBUG("W:", w_index_total, "E:", e_index_total);

  // need to assert that the audio codecs
  //

  assert(container->getAudioCodecs().size() != 0 &&
         w_index_total <= e_index_total);

  assert(container->getAudioCodecs().size() <=
         media->probeResult->audioStreams.size());

#pragma endregion

  for (auto codec : container->getAudioCodecs()) {
    std::string codec_name = codec->getName();
    std::string codec_display_name = codec->getDisplayName();
    std::string channel_layout = codec->getChannelLayout();

    int stream = codec->getIndex();
    int channels = codec->getRunningChannel();
    int sample_rate = codec->getRunningSampleRate();
    int bit_depth = codec->getRunningBitDepth();

    LOG_DEBUG("Audio index [", stream, "] using codec (", codec_name,
              ") with channels (", channels, ") and sample rate (", sample_rate,
              ") and bit depth (", bit_depth, ")");

    LOG_DEBUG("Formatted title for index [", stream,
              "] is: ", codec_display_name, " ", channel_layout);

    result.push_back("-map 0:a:" + std::to_string(stream));

    result.push_back("-c:a:" + std::to_string(stream) + " " + codec_name);
    result.push_back("-ac:a:" + std::to_string(stream) + " " +
                     std::to_string(channels));
    result.push_back("-ar:a:" + std::to_string(stream) + " " +
                     std::to_string(sample_rate));
    result.push_back("-metadata:s:a:" + std::to_string(stream) + " " +
                     " title=\"" + codec_display_name + " " + channel_layout +
                     "\"");
  }

  result.push_back("-map 0:s?");
  result.push_back("-map 0:t?");

  // attachments?
  result.push_back("-c:t copy");

  result.push_back(
      "-c:v " + EnumToStringFactory::get<Encoders>(childOptions.runningEncoder)
                    .getName());

  result.push_back("-preset slow");

  result.push_back("-level 4.1");

  if (argumentRegistry.get_t<FlagArgument>(Command::BITRATE)->get()) {
    result.push_back("-b:v " + std::to_string(format.bitrate) + "M");
    result.push_back("-bufsize " + std::to_string(format.bitrate * 2) + "M");
    result.push_back("-maxrate " + std::to_string(format.max * 2) + "M");
    result.push_back("-minrate " + std::to_string(format.min * 2) + "M");
  } else if (argumentRegistry.get_t<FlagArgument>(Command::CONSTRAIN)->get()) {
    result.push_back("-crf " + std::to_string(format.crf));
    result.push_back("-bufsize " + std::to_string(format.bitrate * 2) + "M");
    result.push_back("-maxrate " + std::to_string(format.max * 2) + "M");
  } else {
    result.push_back("-crf " + std::to_string(format.crf));
  }

  if (argumentRegistry.get_t<FlagArgument>(Command::CROP)->get()) {
    result.push_back("-vf scale=" + media->video->convertedResolution +
                     ":flags=lanczos,crop=" + format.crop);

  }

  else
    result.push_back("-vf scale=" + media->video->convertedResolution +
                     ":flags=lanczos");

  StringArgument *startBeginning =
      argumentRegistry.get_t<StringArgument>("-ss");

  if (!startBeginning->get().empty()) {
    result.push_back("-ss " + startBeginning->get());
  }

  TimeStringVectorArgument *trim =
      argumentRegistry.get_t<TimeStringVectorArgument>("-tr");

  if (!trim->get().empty()) {
    result.push_back("-ss " + trim->get()[0]);
    result.push_back("-to " + trim->get()[1]);
  }

  /** TODO: flesh out later */
  // if (this.video.subtitle_provider) {

  //     if (this.video.subtitle_provider === "mov")
  //     this.ffmpeg_argument.push("-c:s mov_text") else
  //     this.ffmpeg_argument.push("-c:s copy")

  // }

  result.push_back("-c:s copy");

  if (argumentRegistry.get_t<BaseArgument<Tunes>>(Command::TUNE)->get() !=
      Tunes::DEFAULT) {
    result.push_back(
        "-tune " +
        EnumToStringFactory::get<Tunes>(
            argumentRegistry.get_t<BaseArgument<Tunes>>(Command::TUNE)->get())
            .getName());
  }

  result.push_back("\"" + media->file->conversionFilePath + "\"");

  if (false // TODO: create validate flag
      || argumentRegistry.get_t<FlagArgument>(Command::OVERWRITE)->get())
    result.push_back("-y");
  else {
    result.push_back("-n");
  }

  LOG_DEBUG("FFMPEG ARGUMENTS: ", ListUtils::join(result, " "));

  return result;
}
