#include "FFmpegArgumentBuilder.h"

#include <string>
#include <vector>

#include "../../../program/Program.h"
#include "../../../utils/logging/Logger.h"
#include "../../child/media/Media.h"
#include "../../settings/arguments/EnumArgument.h"
#include "../../settings/arguments/FlagArgument.h"
#include "../../settings/arguments/StringArgument.h"
#include "../../settings/arguments/TimeStringVectorArgument.h"
#include "../../settings/arguments/VectorArgument.h"
#include "../../settings/arguments/video/Quality.h"
#include "audio/AudioCodecFactory.h"
#include "container/BaseContainer.h"
#include "container/ContainerFactory.h"
#include "src/program/settings/enums/Command.h"
#include "src/program/settings/enums/EnumToStringFactory.h"
#include "src/utils/logging/LogColor.h"
#include "../../registry/ArgumentRegistry.h"
#include "subtitle/SubtitleCodecFactory.h"
#include "video/VideoCodecFactory.h"
#include "../../settings/enums/Command.h"
#include <cassert>
#include "audio/BaseAudioCodec.h"
#include "../media/MediaFormat.h"
#include "../../settings/arguments/BaseArgument.h"
#include "../../settings/enums/Container.h"
#include "../../settings/enums/Encoders.h"
#include "../../settings/enums/EnumToStringFactory.h"
#include "../../settings/enums/HWAccelerators.h"
#include "../../settings/enums/Tunes.h"
#include "../../settings/options/ChildOptions.h"
#include "../../../utils/ListUtils.h"

FFmpegArgumentBuilder::FFmpegArgumentBuilder(Media* _media)
  : container(nullptr), media(_media) {
  ChildOptions& childOptions = *Program::settings->childOptionsMap[media->id];
  ArgumentRegistry& argumentRegistry = *childOptions.argumentRegistry;
  MediaFormat format = argumentRegistry.get_t<Quality>(Command::QUALITY)->get();

  Encoders wanted_enc =
    argumentRegistry.get_t<EnumArgument<Encoders>>(Command::ENCODER)->get();
  Container wanted_container =
    argumentRegistry.get_t<EnumArgument<Container>>(Command::CONTAINER)
    ->get();

  // audio streams desired by the user
  VectorArgument<int>* audioStreams =
    argumentRegistry.get_t<VectorArgument<int>>(Command::AUDIOSTREAMS);

  // TODO: either use enums for codecs or strings for everything

  auto videoCodec =
    VideoCodecFactory::create(EnumToStringFactory::get(wanted_enc));
  auto audioCodecs = std::vector<BaseAudioCodec*>();

  this->container =
    ContainerFactory::create(EnumToStringFactory::get(wanted_container));

  // TODO: make this user adjustable
  auto subtitleCodec =
    SubtitleCodecFactory::create(container->fallbackSubtitleCodec());

  LOG_DEBUG("Container ptr: {}", static_cast<void*>(container));
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

  if (w_index_total > e_index_total) {
    LOG_DEBUG("WANTED INDEX TOTAL IS GREATER THAN EXISTING INDEX TOTAL");
    LOG_DEBUG("WANTED INDEX TOTAL: ", w_index_total);
    LOG_DEBUG("EXISTING INDEX TOTAL: ", e_index_total);
    LOG(LogColor::fgRed("Wanted audio indexes exceed existing audio streams."));
    Program::stopFlag = true;
    // throw std::invalid_argument(
    //     "Wanted audio indexes exceed existing audio streams.");
  }

#pragma endregion

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

  // we are generating a new BaseAudioCodec for each audio stream
  // in the existing audio streams
  for (int i = 0; i < media->probeResult->audioStreams.size(); i++) {
    BaseAudioCodec* audioCodec = nullptr;
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
      audioCodec = AudioCodecFactory::create(wanted_codecs[i]);
    }
    // else use the last codec in the list
    else if (!wanted_codecs.empty()) {
      LOG_DEBUG("Audio index [", i, "] exceeded codecs, using last codec (",
        wanted_codecs[wanted_codecs.size() - 1], ")");
      audioCodec =
        AudioCodecFactory::create(wanted_codecs[wanted_codecs.size() - 1]);
    }
    // else copy the codec
    else {
      LOG_DEBUG("Audio index [", i, "] marked for copy codec.");
      audioCodec = AudioCodecFactory::create(media_audio_codec);
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
    else if (!wanted_channels.empty()) {
      LOG_DEBUG("Audio index [", i, "] exceeded channels, using last channel (",
        wanted_channels[wanted_channels.size() - 1], ")");
      audioCodec->setChannel(wanted_channels[wanted_channels.size() - 1]);
    }
    // copy channels
    else {
      LOG_DEBUG("Audio index [", i, "] marked for copy channels.");
      audioCodec->setChannel(media_channels);
    }

    // set index
    audioCodec->setIndex(audioCodecs.size());
    audioCodec->setMapIndex(i);

    // push to audio codecs
    audioCodecs.push_back(audioCodec);
  }
#pragma endregion AUDIO SETTINGS

  this->container->setSubtitleCodec(subtitleCodec);
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

void FFmpegArgumentBuilder::validate() { return; }

std::vector<std::string> FFmpegArgumentBuilder::build() {

  assert(container->getAudioCodecs().size() <=
    media->probeResult->audioStreams.size());

  // ProgramOptions& programSettings = *Program::settings->programOptions;
  ChildOptions& childOptions = *Program::settings->childOptionsMap[media->id];
  ArgumentRegistry& argumentRegistry = *childOptions.argumentRegistry;
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

  for (auto codec : container->getAudioCodecs()) {
    std::string codec_name = codec->getName();
    std::string codec_display_name = codec->getDisplayName();
    std::string channel_layout = codec->getChannelLayout();

    int as_index = codec->getIndex();
    int as_map_index = codec->getMapIndex();
    int channels = codec->getRunningChannel();
    int sample_rate = codec->getRunningSampleRate();
    int bit_depth = codec->getRunningBitDepth();

    LOG_DEBUG("Audio index [", as_index, "] mapped at [", as_map_index,
      "] using codec (", codec_name, ") with channels (", channels,
      ") and sample rate (", sample_rate, ") and bit depth (",
      bit_depth, ")");

    LOG_DEBUG("Formatted title for index [", as_index,
      "] is:", codec_display_name, channel_layout);

    result.push_back("-map 0:a:" + std::to_string(as_map_index));

    result.push_back("-c:a:" + std::to_string(as_index) + " " + codec_name);
    result.push_back("-ac:a:" + std::to_string(as_index) + " " +
      std::to_string(channels));
    result.push_back("-ar:a:" + std::to_string(as_index) + " " +
      std::to_string(sample_rate));
    result.push_back("-metadata:s:a:" + std::to_string(as_index) + " " +
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
  }
  else if (argumentRegistry.get_t<FlagArgument>(Command::CONSTRAIN)->get()) {
    result.push_back("-crf " + std::to_string(format.crf));
    result.push_back("-bufsize " + std::to_string(format.bitrate * 2) + "M");
    result.push_back("-maxrate " + std::to_string(format.max * 2) + "M");
  }
  else {
    result.push_back("-crf " + std::to_string(format.crf));
  }

  if (argumentRegistry.get_t<FlagArgument>(Command::CROP)->get()) {
    result.push_back("-vf scale=" + media->video->convertedResolution +
      ":flags=lanczos,crop=" + format.crop);

  }

  else
    result.push_back("-vf scale=" + media->video->convertedResolution +
      ":flags=lanczos");

  StringArgument* startBeginning =
    argumentRegistry.get_t<StringArgument>(Command::START);

  if (!startBeginning->get().empty()) {
    result.push_back("-ss " + startBeginning->get());
  }

  TimeStringVectorArgument* trim =
    argumentRegistry.get_t<TimeStringVectorArgument>(Command::TRIM);

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

  std::string subtitle_name = this->container->getSubtitleCodec()->getName();
  result.push_back("-c:s " + subtitle_name);

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
