#include "FFmpegArgumentBuilder.h"

#include <string>

#include "src/program/Program.h"
#include "src/program/child/media/Media.h"
#include "src/program/child/media/MediaFormat.h"
#include "src/program/ffmpeg/attachment/AttachmentFactory.h"
#include "src/program/ffmpeg/audio/AudioCodecFactory.h"
#include "src/program/ffmpeg/audio/BaseAudioCodec.h"
#include "src/program/ffmpeg/container/BaseContainer.h"
#include "src/program/ffmpeg/container/ContainerFactory.h"
#include "src/program/ffmpeg/probe/ProbeResultStreamAttachment.h"
#include "src/program/ffmpeg/subtitle/SubtitleCodecFactory.h"
#include "src/program/ffmpeg/video/VideoCodecFactory.h"
#include "src/program/registry/ArgumentRegistry.h"
#include "src/program/settings/arguments/StringArgument.h"
#include "src/program/settings/arguments/TimeStringVectorArgument.h"
#include "src/program/settings/arguments/VectorArgument.h"
#include "src/program/settings/enums/Command_N.h"
#include "src/program/settings/enums/Container_N.h"
#include "src/program/settings/enums/Encoders_N.h"
#include "src/program/settings/enums/HWAccelerators_N.h"
#include "src/utils/ListUtils.h"
#include "src/utils/logging/LogColor.h"
#include "src/utils/logging/Logger.h"
#include <cassert>

FFmpegArgumentBuilder::FFmpegArgumentBuilder(Media *_media)
    : container(nullptr), media(_media) {
  Arguments &arguments = _media->getArguments();
  ArgumentRegistry &argumentRegistry = arguments.argumentRegistry;
  MediaFormat format = argumentRegistry.get_p<Command_N::QUALITY>().get();

  Encoders_N::Encoders wanted_enc =
      argumentRegistry.get_p<Command_N::ENCODER>().get();
  Container_N::Container wanted_container =
      argumentRegistry.get_p<Command_N::CONTAINER>().get();

  // audio streams desired by the user
  const VectorArgument<int> audioStreams =
      argumentRegistry.get_p<Command_N::AUDIOSTREAMS>();

  const VectorArgument<int> audioBitrates =
      argumentRegistry.get_p<Command_N::AUDIOBITRATE>();

  const VectorArgument<int> audioChannels =
      argumentRegistry.get_p<Command_N::AUDIOCHANNELS>();

  const VectorArgument<std::string> audioBitDepths =
      argumentRegistry.get_p<Command_N::AUDIOBITDEPTH>();

  const VectorArgument<int> audioSampleRates =
      argumentRegistry.get_p<Command_N::AUDIOSAMPLERATE>();

  const VectorArgument<int> subtitleStreams =
      argumentRegistry.get_p<Command_N::SUBTITLESTREAMS>();

  // TODO: either use enums for codecs or strings for everything

  auto videoCodec =
      VideoCodecFactory::create(Encoders_N::definition(wanted_enc));
  auto audioCodecs = std::vector<BaseAudioCodec *>();
  auto subtitleCodecs = std::vector<BaseSubtitleCodec *>();

  this->container =
      ContainerFactory::create(Container_N::definition(wanted_container));

  LOG_DEBUG(Logger::Priority::INFO, "Container ptr: {}",
            static_cast<void *>(container));

  // assert audio stream mapping
  assertStreamMapping(audioStreams, media->probeResult->audioStreams);
  // assert audio bitrate stream mapping
  assertStreamMapping(audioBitrates, media->probeResult->audioStreams);
  // assert audio channels stream mapping
  assertStreamMapping(audioChannels, media->probeResult->audioStreams);
  // assert audio bit depth stream mapping
  assertStreamMapping(audioBitDepths, media->probeResult->audioStreams);
  // assert audio sample rate stream mapping
  assertStreamMapping(audioSampleRates, media->probeResult->audioStreams);

  // assert subtitle stream mapping
  assertStreamMapping(subtitleStreams, media->probeResult->subtitleStreams);

  this->container->setSubtitleCodec(
      generateSubtitleCodecs(subtitleStreams, argumentRegistry));
  this->container->setAudioCodec(
      generateAudioCodecs(audioStreams, argumentRegistry));
  this->container->setVideoCodec(videoCodec);
  this->container->setAttachments(generateAttachments());
}

FFmpegArgumentBuilder::~FFmpegArgumentBuilder() {
  LOG_DEBUG(Logger::Priority::INFO, "Deconstructing...");
  if (container != nullptr) {
    LOG_DEBUG(Logger::Priority::INFO, "Deconstructing Container.");
    delete container;
  }
}

std::vector<BaseAudioCodec *>
FFmpegArgumentBuilder::generateAudioCodecs(const VectorArgument<int> &streams,
                                           ArgumentRegistry &argumentRegistry) {
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

  std::vector<BaseAudioCodec *> audioCodecs;

  std::vector<std::string> wanted_codecs =
      argumentRegistry.get_p<Command_N::AUDIOCODEC>().get();
  std::vector<int> wanted_bitrates =
      argumentRegistry.get_p<Command_N::AUDIOBITRATE>().get();
  std::vector<int> wanted_channels =
      argumentRegistry.get_p<Command_N::AUDIOCHANNELS>().get();
  std::vector<std::string> wanted_bit_depths =
      argumentRegistry.get_p<Command_N::AUDIOBITDEPTH>().get();
  std::vector<int> wanted_sample_rates =
      argumentRegistry.get_p<Command_N::AUDIOSAMPLERATE>().get();

  // we are generating a new BaseAudioCodec for each audio stream
  // in the existing audio streams
  for (u_long i = 0; i < media->probeResult->audioStreams.size(); i++) {
    BaseAudioCodec *audioCodec = nullptr;
    int media_channels = media->probeResult->audioStreams[i].channels;
    int media_bitrate =
        static_cast<int>(media->probeResult->audioStreams[i].tags.BPS / 1000);
    std::string media_bit_depth =
        media->probeResult->audioStreams[i].sample_fmt;
    int media_sample_rate = media->probeResult->audioStreams[i].sample_rate;
    std::string media_audio_codec =
        media->probeResult->audioStreams[i].codec_name;

    // skip if stream index isnt in the
    // desired list
    if (!streams.get().empty()) {
      // and if the audio stream is not in the list
      // skip the audio stream
      if (!ListUtils::contains(streams.get(), (int)i)) {
        LOG_DEBUG(Logger::Priority::INFO, "Skipping audio stream", i,
                  "for not being in the list.");
        continue;
      }
    }

    LOG_DEBUG(Logger::Priority::INFO, "Audio stream", i, "is in the list.");

    /***************************************************
     *                                                 *
     *              AUDIO CODEC SETTINGS               *
     *                                                 *
     ***************************************************/

    // if there are more audio codecs than streams
    if (wanted_codecs.size() > i) {
      LOG_DEBUG(Logger::Priority::INFO, "Audio index [", i,
                "] is using codec (", wanted_codecs[i], ")");
      audioCodec = AudioCodecFactory::create(wanted_codecs[i]);
    }
    // else use the last codec in the list
    else if (!wanted_codecs.empty()) {
      LOG_DEBUG(Logger::Priority::INFO, "Audio index [", i,
                "] exceeded codecs, using last codec (",
                wanted_codecs[wanted_codecs.size() - 1], ")");
      audioCodec =
          AudioCodecFactory::create(wanted_codecs[wanted_codecs.size() - 1]);
    }
    // else copy the codec
    else {
      LOG_DEBUG(Logger::Priority::INFO, "Audio index [", i,
                "] marked for copy codec.");
      audioCodec = AudioCodecFactory::create(media_audio_codec);
      // set bit depth
      audioCodec->setBitDepth(media->probeResult->audioStreams[i].sample_fmt);
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
      LOG_DEBUG(Logger::Priority::INFO, "Audio index [", i,
                "] is using channel (", wanted_channels[i], ")");
      audioCodec->setChannel(wanted_channels[i]);
    } else if (!wanted_channels.empty()) {
      LOG_DEBUG(Logger::Priority::INFO, "Audio index [", i,
                "] exceeded channels, using last channel (",
                wanted_channels[wanted_channels.size() - 1], ")");
      audioCodec->setChannel(wanted_channels[wanted_channels.size() - 1]);
    }
    // copy channels
    else {
      LOG_DEBUG(Logger::Priority::INFO, "Audio index [", i,
                "] marked for copy channels.");
      audioCodec->setChannel(media_channels);
    }

    /***************************************************
     *                                                 *
     *            AUDIO BITRATE SETTINGS               *
     *                                                 *
     ***************************************************/

    if (wanted_bitrates.size() > i) {
      LOG_DEBUG(Logger::Priority::INFO, "Audio index [", i,
                "] is using bitrate (", wanted_bitrates[i], ")");
      audioCodec->setBitrate(wanted_bitrates[i]);
    } else if (!wanted_bitrates.empty()) {
      LOG_DEBUG(Logger::Priority::INFO, "Audio index [", i,
                "] exceeded bitrates, using last bitrate (",
                wanted_bitrates[wanted_bitrates.size() - 1], ")");
      audioCodec->setBitrate(wanted_bitrates[wanted_bitrates.size() - 1]);
    }
    // copy bitrate
    else {
      LOG_DEBUG(Logger::Priority::INFO, "Audio index [", i,
                "] marked for copy bitrate.");
      audioCodec->setBitrate(media_bitrate);
    }

    /***************************************************
     *                                                 *
     *            AUDIO BIT DEPTH SETTINGS             *
     *                                                 *
     ***************************************************/

    if (wanted_bit_depths.size() > i) {
      LOG_DEBUG(Logger::Priority::INFO, "Audio index [", i,
                "] is using bit depth (", wanted_bit_depths[i], ")");
      audioCodec->setBitDepth(wanted_bit_depths[i]);
    } else if (!wanted_bit_depths.empty()) {
      LOG_DEBUG(Logger::Priority::INFO, "Audio index [", i,
                "] exceeded bit depths, using last bit depth (",
                wanted_bit_depths[wanted_bit_depths.size() - 1], ")");
      audioCodec->setBitDepth(wanted_bit_depths[wanted_bit_depths.size() - 1]);
    }
    // copy bit depth
    else {
      LOG_DEBUG(Logger::Priority::INFO, "Audio index [", i,
                "] marked for copy bit depth.");
      audioCodec->setBitDepth(media_bit_depth);
    }

    /***************************************************
     *                                                 *
     *            AUDIO SAMPLE RATE SETTINGS           *
     *                                                 *
     ***************************************************/

    if (wanted_sample_rates.size() > i) {
      LOG_DEBUG(Logger::Priority::INFO, "Audio index [", i,
                "] is using sample rate (", wanted_sample_rates[i], ")");
      audioCodec->setSampleRate(wanted_sample_rates[i]);
    } else if (!wanted_sample_rates.empty()) {
      LOG_DEBUG(Logger::Priority::INFO, "Audio index [", i,
                "] exceeded sample rates, using last sample rate (",
                wanted_sample_rates[wanted_sample_rates.size() - 1], ")");
      audioCodec->setSampleRate(
          wanted_sample_rates[wanted_sample_rates.size() - 1]);
    }
    // copy sample rate
    else {
      LOG_DEBUG(Logger::Priority::INFO, "Audio index [", i,
                "] marked for copy sample rate.");
      audioCodec->setSampleRate(media_sample_rate);
    }

    // set index
    audioCodec->setIndex(audioCodecs.size());
    audioCodec->setMapIndex(i);

    // push to audio codecs
    audioCodecs.push_back(audioCodec);
  }

  return audioCodecs;
}

std::vector<BaseSubtitleCodec *> FFmpegArgumentBuilder::generateSubtitleCodecs(
    const VectorArgument<int> &streams, ArgumentRegistry &argumentRegistry) {
  // Implementation for generating subtitle codecs
  std::vector<BaseSubtitleCodec *> subtitleCodecs;

  std::vector<std::string> wanted_codecs =
      argumentRegistry.get_p<Command_N::SUBTITLECODECS>().get();

  for (u_long i = 0; i < media->probeResult->subtitleStreams.size(); i++) {
    BaseSubtitleCodec *subtitleCodec = nullptr;
    std::string media_subtitle_codec =
        media->probeResult->subtitleStreams[i].codec_name;

    // skip if stream index isnt in the
    // desired list
    if (!streams.get().empty()) {
      // and if the subtitle stream is not in the list
      if (!ListUtils::contains(streams.get(), (int)i)) {
        LOG_DEBUG(Logger::Priority::INFO, "Skipping subtitle stream", i,
                  "for not being in the list.");
        continue;
      }
    }

    LOG_DEBUG(Logger::Priority::INFO, "Subtitle stream", i, "is in the list.");

    // existing codec
    auto ec = SubtitleCodecFactory::create(
        media->probeResult->subtitleStreams[i].codec_name);

    // if there are more subtitle codecs than streams
    if (wanted_codecs.size() > i) {
      LOG_DEBUG(Logger::Priority::INFO, "Subtitle index [", i,
                "] is using codec (", wanted_codecs[i], ")");

      auto wc = SubtitleCodecFactory::create(wanted_codecs[i]);

      if ((!ec->isImage() && wc->isImage()) ||
          (ec->isImage() && !wc->isImage())) {
        LOG(Logger::Priority::INFO,
            LogColor::fgRed("Cannot convert subtitle (" + ec->getName() +
                            ") to (" + wc->getName() + ")."));
        LOG(Logger::Priority::INFO,
            LogColor::fgRed(ec->getName() + " was a " +
                            (ec->isImage() ? "image" : "text") +
                            " format but " + wc->getName() + " is a " +
                            (wc->isImage() ? "image" : "text") + " format."));
        Program::stopFlag = true;
        delete wc;
        delete ec;
        return subtitleCodecs;
      }

      subtitleCodec = wc;

    }
    // else use the last codec in the list
    else if (!wanted_codecs.empty()) {
      LOG_DEBUG(Logger::Priority::INFO, "Subtitle index [", i,
                "] exceeded codecs, using last codec (",
                wanted_codecs[wanted_codecs.size() - 1], ")");

      auto wc = subtitleCodecs[subtitleCodecs.size() - 1];

      if ((!ec->isImage() && wc->isImage()) ||
          (ec->isImage() && !wc->isImage())) {
        LOG(Logger::Priority::INFO,
            LogColor::fgRed("Cannot convert subtitle (" + ec->getName() +
                            ") to (" + wc->getName() + ")."));
        LOG(Logger::Priority::INFO,
            LogColor::fgRed(ec->getName() + " was a " +
                            (ec->isImage() ? "image" : "text") +
                            " format but " + wc->getName() + " is a " +
                            (wc->isImage() ? "image" : "text") + " format."));
        Program::stopFlag = true;
        delete wc;
        delete ec;
        return subtitleCodecs;
      }

      subtitleCodec = SubtitleCodecFactory::create(wc->getName());

    }
    // else copy the codec
    else {
      LOG_DEBUG(Logger::Priority::INFO, "Subtitle index [", i,
                "] marked for copy codec.");
      subtitleCodec = SubtitleCodecFactory::create(media_subtitle_codec);
    }

    subtitleCodec->setIndex(subtitleCodecs.size());
    subtitleCodec->setMapIndex(i);

    subtitleCodecs.push_back(subtitleCodec);
    delete ec;
  }

  return subtitleCodecs;
}

std::vector<BaseAttachment *> FFmpegArgumentBuilder::generateAttachments() {
  std::vector<BaseAttachment *> attachments;

  for (u_long i = 0; i < media->probeResult->attachmentStreams.size(); i++) {
    ProbeResultStreamAttachment att = media->probeResult->attachmentStreams[i];
    attachments.push_back(AttachmentFactory::create(att.tags.mimetype));
  }

  return attachments;
}

void FFmpegArgumentBuilder::validate() { return; }
void FFmpegArgumentBuilder::term() {
  LOG_DEBUG(Logger::Priority::INFO, "Terminating program.");
  Program::stopFlag = true;
}

std::vector<std::string> FFmpegArgumentBuilder::build() {

  assert(container->getAudioCodecs().size() <=
         media->probeResult->audioStreams.size());

  if (container->getAudioCodecs().size() <= 0) {
    term();
    return {};
  }

  // ProgramOptions& programSettings = *Program::settings->programOptions;
  Arguments &arguments = media->getArguments();
  ArgumentRegistry &argumentRegistry = arguments.argumentRegistry;
  MediaFormat format = argumentRegistry.get_p<Command_N::QUALITY>().get();

  std::vector<std::string> result;

  result.push_back("-v error -stats");

  if (argumentRegistry.get_p<Command_N::HARDWAREDECODE>()) {
    if (arguments.running_hw_accel != HWAccelerators_N::INVALID) {
      result.push_back("-hwaccel " + HWAccelerators_N::definition(
                                         arguments.running_hw_accel));
    }
  }

  result.push_back("-i \"" +
                   media->getFile().naming.original_full_path.string() + "\"");

  result.push_back("-metadata title=\"" +
                   media->getFile().naming.conversion_name + "\"");

  result.push_back("-map 0:v:0");

  for (auto codec : container->getAudioCodecs()) {
    std::string codec_name = codec->getName();
    std::string codec_display_name = codec->getDisplayName();
    std::string channel_layout = codec->getChannelLayout();

    int as_index = codec->getIndex();
    int as_map_index = codec->getMapIndex();
    int channels = codec->getRunningChannel();
    int sample_rate = codec->getRunningSampleRate();
    std::string bit_depth = codec->getRunningBitDepth();

    LOG_DEBUG(Logger::Priority::INFO, "Audio index [", as_index,
              "] mapped at [", as_map_index, "] using codec (", codec_name,
              ") with channels (", channels, ") and sample rate (", sample_rate,
              ") and bit depth (", bit_depth, ")", " and bitrate (",
              codec->getRunningBitrate(), ")");

    LOG_DEBUG(Logger::Priority::INFO, "Formatted title for index [", as_index,
              "] is:", codec_display_name, channel_layout);

    result.push_back("-map 0:a:" + std::to_string(as_map_index));
    result.push_back("-sample_fmt:a:" + std::to_string(as_index) + " " +
                     bit_depth);
    result.push_back("-c:a:" + std::to_string(as_index) + " " + codec_name);
    result.push_back("-ac:a:" + std::to_string(as_index) + " " +
                     std::to_string(channels));
    result.push_back("-ar:a:" + std::to_string(as_index) + " " +
                     std::to_string(sample_rate));
    result.push_back("-b:a:" + std::to_string(as_index) + " " +
                     std::to_string(codec->getRunningBitrate()) + "k");
    result.push_back("-metadata:s:a:" + std::to_string(as_index) + " " +
                     " title=\"" + codec_display_name + " " + channel_layout +
                     "\"");
  }

  for (auto codec : container->getSubtitleCodecs()) {

    int ss_index = codec->getIndex();
    int ss_map_index = codec->getMapIndex();

    LOG_DEBUG(Logger::Priority::INFO, "Subtitle index [", ss_index,
              "] mapped at [", ss_map_index, "] using codec (",
              codec->getName(), ")");

    result.push_back("-map 0:s:" + std::to_string(ss_map_index));

    result.push_back("-c:s:" + std::to_string(ss_index) + " " +
                     codec->getArg());
  }

  for (auto attachment : container->getAttachments()) {
    int att_index = attachment->getIndex();
    int att_map_index = attachment->getMapIndex();

    LOG_DEBUG(Logger::Priority::INFO, "Attachment index [", att_index,
              "] mapped at [", att_map_index, "] using attachment (",
              attachment->getName(), ")");

    result.push_back("-map 0:t:" + std::to_string(att_map_index));
    result.push_back("-c:t:" + std::to_string(att_index) + " copy");
  }

  result.push_back("-c:v " + Encoders_N::definition(arguments.running_encoder));

  result.push_back("-preset slow");

  result.push_back("-level 4.1");

  // TODO: refactor to handle the differing control rate flags
  // for each codec
  if (argumentRegistry.get_p<Command_N::BITRATE>()) {
    result.push_back("-b:v " + std::to_string(format.bitrate) + "M");
    result.push_back("-bufsize " + std::to_string(format.bitrate * 2) + "M");
    result.push_back("-maxrate " + std::to_string(format.max * 2) + "M");
    result.push_back("-minrate " + std::to_string(format.min * 2) + "M");
  } else if (argumentRegistry.get_p<Command_N::CONSTRAIN>()) {
    result.push_back(container->getVideoCodec()->controlRateFlag() + " " +
                     std::to_string(format.crf));
    result.push_back("-bufsize " + std::to_string(format.bitrate * 2) + "M");
    result.push_back("-maxrate " + std::to_string(format.max * 2) + "M");
  } else {
    result.push_back(container->getVideoCodec()->controlRateFlag() + " " +
                     std::to_string(format.crf));
  }

  if (argumentRegistry.get_p<Command_N::CROP>()) {
    result.push_back(
        "-vf scale=" + media->getFile().video_info.convertedResolution +
        ":flags=lanczos,crop=" + format.crop);

  }

  else
    result.push_back(
        "-vf scale=" + media->getFile().video_info.convertedResolution +
        ":flags=lanczos");

  StringArgument startBeginning = argumentRegistry.get_p<Command_N::START>();

  if (!startBeginning.get().empty()) {
    result.push_back("-ss " + startBeginning.get());
  }

  TimeStringVectorArgument trim = argumentRegistry.get_p<Command_N::TRIM>();

  if (!trim.get().empty()) {
    result.push_back("-ss " + trim.get()[0]);
    result.push_back("-to " + trim.get()[1]);
  }

  /** TODO: flesh out later */
  // if (this.video.subtitle_provider) {

  //     if (this.video.subtitle_provider === "mov")
  //     this.ffmpeg_argument.push("-c:s mov_text") else
  //     this.ffmpeg_argument.push("-c:s copy")

  // }

  if (argumentRegistry.get_p<Command_N::TUNE>() != Tunes_N::DEFAULT) {
    result.push_back(
        "-tune " +
        Tunes_N::definition(argumentRegistry.get_p<Command_N::TUNE>().get()));
  }

  result.push_back(
      "\"" + media->getFile().naming.conversion_full_path.string() + "\"");

  if (false // TODO: create validate flag
      || argumentRegistry.get_p<Command_N::OVERWRITE>().get())
    result.push_back("-y");
  else {
    result.push_back("-n");
  }

  LOG_DEBUG(Logger::Priority::INFO,
            "FFMPEG ARGUMENTS: ", ListUtils::join(result, " "));

  return result;
}
