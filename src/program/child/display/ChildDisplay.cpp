#include "ChildDisplay.h"

#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <queue>
#include <string>

#include "src/program/child/Child.h"
#include "src/program/child/media/Media.h"
#include "src/program/context/Arguments.h"
#include "src/program/definitions/DefinitionRegistry.h"
#include "src/program/ffmpeg/probe/ProbeResultStreamAudio.h"
#include "src/program/ffmpeg/probe/ProbeResultStreamSubtitle.h"
#include "src/program/ffmpeg/probe/ProbeResultStreamVideo.h"
#include "src/program/generics/JSONSerializableRunner.h"
#include "src/program/settings/enums/Command_N.h"
#include "src/program/settings/enums/Container_N.h"
#include "src/program/settings/enums/Tunes_N.h"
#include "src/program/ticker/NTicker.h"
#include "src/utils/StringUtils.h"
#include "src/utils/TimeUtils.h"
#include "src/utils/logging/LogColor.h"
#include "src/utils/logging/Logger.h"

// TODO: appending to string rebuilds each time
//  CONT: we should do a sstream

// NOLINTBEGIN(modernize-return-braced-init-list)
inline auto tab(int spaces) -> std::string {
  return std::string(spaces * 2, ' ');
}
// NOLINTEND(modernize-return-braced-init-list)

void ChildDisplay::print(JSONSerializableRunner &ticker) {
  Child &child = *ticker.getRunner<NTicker>()->runner->getRunner<Child>();

  Arguments &arguments = child.getArguments();
  ArgumentRegistry &argumentRegistry = arguments.argumentRegistry;

  std::string sendStr = "";

  // int bufferLen = static_cast<int>(child.converting.size()) +
  //                 static_cast<int>(child.pending.size()) + 1;

  std::string time = StringUtils::bracket(
      "TIME", TimeUtils::dateFormat(TimeUtils::getEpoch()), LogColor::fgGray,
      LogColor::fgCyan, LogColor::fgWhite);

  std::string encoder = StringUtils::bracket(
      "TARGET ENC", DefinitionRegistry::defFromEnum(
                        argumentRegistry.get<Command_N::ENCODER>().get()));

  std::string resolution = StringUtils::bracket(
      "RES", argumentRegistry.get<Command_N::QUALITY>().get().name);

  std::string tune = StringUtils::bracket(
      "TUNE", Tunes_N::definition(argumentRegistry.get<Command_N::TUNE>()));

  std::string amount = StringUtils::bracket(
      "AMOUNT", argumentRegistry.get<Command_N::AMOUNT>().toString());
  std::string container = StringUtils::bracket(
      "CONTAINER",
      Container_N::definition(argumentRegistry.get<Command_N::CONTAINER>()));

  // std::string a = ArgumentRegistry::get<IntegerArgument>("-a");

  std::string constrain =
      StringUtils::bracket("CONSTRAIN", "", LogColor::fgGray, LogColor::fgRed);
  std::string debug =
      StringUtils::bracket("DEBUG", "", LogColor::fgGray, LogColor::fgRed);
  std::string crop =
      StringUtils::bracket("CROP", "", LogColor::fgGray, LogColor::fgRed);

  std::string header = time + " " + encoder + " " + " " + resolution + " " +
                       tune + " " + amount + " " + container;

  if (argumentRegistry.get<Command_N::CONSTRAIN>()) {
    header += " " + constrain;
  }

  if (argumentRegistry.get<Command_N::CROP>()) {
    header += " " + crop;
  }

  // clear console?
#ifdef _WIN32
  system("cls");
#else
  system("clear");
#endif
  sendStr += header + "\n";
  // Program::log->sendBuffer(bufferLen, header);

  std::queue<Media *> t_queue;

  while (!child.converting.empty()) {
    Media *media = child.converting.front();
    child.converting.pop();

    sendStr += media->convertingLine();

    t_queue.push(media);
  }

  child.converting = t_queue;
  t_queue = std::queue<Media *>();

  while (!child.pending.empty()) {
    Media *media = child.pending.front();
    child.pending.pop();

    sendStr += media->pendingLine();

    t_queue.push(media);
  }
  child.pending = t_queue;
  LOG(Logger::Priority::INFO, sendStr);
}

void ChildDisplay::printDebug(JSONSerializableRunner &ticker) {
  Child &child = *ticker.getRunner<NTicker>()->runner->getRunner<Child>();

  std::queue<Media *> t_queue;

  while (!child.converting.empty()) {
    Media *media = child.converting.front();
    child.converting.pop();
    auto totalFrames =
        static_cast<double>(media->getFile().video_info.totalFrames);
    auto completedFrames =
        static_cast<double>(media->getFile().processing_info.completedFrames);

    int percent_result =
        static_cast<int>(std::round((completedFrames / totalFrames) * 100));

    LOG_DEBUG(Logger::Priority::INFO,
              "TOTAL_FRAMES:" + std::to_string(totalFrames));
    LOG_DEBUG(Logger::Priority::INFO,
              "COMPLETED_FRAMES:" + std::to_string(completedFrames));
    LOG_DEBUG(Logger::Priority::INFO,
              "PERCENT_RESULT:" + std::to_string(percent_result));

    t_queue.push(media);
  }

  child.converting = t_queue;
}

void ChildDisplay::printInformationTyped(NTicker *ticker, Child *child) {
  assert(ticker != nullptr);
  assert(child != nullptr);

  LOG_DEBUG(Logger::Priority::INFO, "Printing Information");

#ifdef _WIN32
  system("cls");
#else
  system("clear");
#endif

  LOG(Logger::Priority::INFO, LogColor::fgBlack("Black"),
      LogColor::fgRed("Red"), LogColor::fgGreen("Green"),
      LogColor::fgGray("Gray"), LogColor::fgYellow("Yellow"),
      LogColor::fgBlue("Blue"), LogColor::fgOrange("Orange"),
      LogColor::fgMagenta("Magenta"), LogColor::fgCyan("Cyan"),
      LogColor::fgWhite("White"));

  if (child->pending.empty()) {
    LOG(Logger::Priority::WARNING, LogColor::fgRed("No media files found."));
    return;
  }

  std::queue<Media *> media_t_queue;
  while (!child->pending.empty()) {
    Arguments &arguments = child->getArguments();

    Media *media = child->pending.front();
    child->pending.pop();

    media->doStatistics();
    media->buildFFmpegArguments(false);

    std::string nl = "\n";
    std::string ob = LogColor::fgGray("[");
    std::string cb = LogColor::fgGray("]");
    std::string colon = LogColor::fgGray(": ");

    LOG(Logger::Priority::INFO, "");
    // parent bloc
    LOG(Logger::Priority::INFO,
        ob + "CWD - " + LogColor::fgGray(arguments.CWD) + cb);
    LOG(Logger::Priority::INFO, "");
    // file bloc
    LOG(Logger::Priority::INFO,
        tab(1) + ob +
            LogColor::fgRed(media->getFile().naming.original_name_ext) + cb);
    LOG(Logger::Priority::INFO, "");
    // format bloc
    LOG(Logger::Priority::INFO, tab(2) + ob + LogColor::fgWhite("Format") + cb);

    LOG(Logger::Priority::INFO,
        tab(3) + LogColor::bgBlue("Duration") + colon +
            LogColor::bgOrange(media->probeResult->format.duration));

    LOG(Logger::Priority::INFO,
        tab(3) + LogColor::bgBlue("Format") + colon +
            LogColor::bgOrange(media->probeResult->format.format_name));

    LOG(Logger::Priority::INFO,
        tab(3) + LogColor::bgBlue("Bit Rate") + colon +
            LogColor::bgOrange(media->probeResult->format.bit_rate));

    LOG(Logger::Priority::INFO,
        tab(3) + LogColor::bgBlue("Size") + colon +
            LogColor::bgOrange(media->probeResult->format.size));

    LOG(Logger::Priority::INFO,
        tab(3) + LogColor::bgBlue("Stream Count") + colon +
            LogColor::bgOrange(
                std::to_string(media->probeResult->format.nb_streams)));
    LOG(Logger::Priority::INFO, "");
    // video stream bloc
    LOG(Logger::Priority::INFO,
        tab(2) + ob + LogColor::fgWhite("Video Streams") + cb);

    // iterate video streams
    for (int i = 0; i < media->probeResult->videoStreams.size(); i++) {
      ProbeResultStreamVideo prsv = media->probeResult->videoStreams[i];

      // codec bloc
      LOG(Logger::Priority::INFO, tab(3) + ob + std::to_string(i) + cb + " " +
                                      LogColor::fgOrange(prsv.codec_name));

      LOG(Logger::Priority::INFO,
          tab(4) + LogColor::fgBlue("Profile") + colon +
              LogColor::fgOrange(prsv.profile.empty() ? "None" : prsv.profile));

      LOG(Logger::Priority::INFO,
          tab(4) + LogColor::fgBlue("Res") + colon +
              LogColor::fgOrange(std::to_string(prsv.width) + "x" +
                                 std::to_string(prsv.height)) +
              " (" + LogColor::fgOrange(prsv.display_aspect_ratio) + ")");

      LOG(Logger::Priority::INFO,
          tab(4) + LogColor::fgBlue("Listed Duration") + colon +
              LogColor::fgOrange(
                  prsv.tags.DURATION.empty() ? "None" : prsv.tags.DURATION));
    }

    LOG(Logger::Priority::INFO, "");
    // audio streams bloc
    LOG(Logger::Priority::INFO,
        tab(2) + ob + LogColor::fgWhite("Audio Streams") + cb);

    // iterate audio streams
    for (int i = 0; i < media->probeResult->audioStreams.size(); i++) {
      ProbeResultStreamAudio prsa = media->probeResult->audioStreams[i];

      // audio lang bloc
      LOG(Logger::Priority::INFO, tab(3) + ob + std::to_string(i) + cb + " " +
                                      LogColor::fgOrange(prsa.tags.language));

      LOG(Logger::Priority::INFO,
          tab(4) + LogColor::fgBlue("Title") + colon +
              LogColor::fgOrange(prsa.tags.title.empty() ? "None"
                                                         : prsa.tags.title));

      LOG(Logger::Priority::INFO, tab(4) + LogColor::fgBlue("Codec") + colon +
                                      LogColor::fgOrange(prsa.codec_name));

      LOG(Logger::Priority::INFO,
          tab(4) + LogColor::fgBlue("Channels") + colon +
              LogColor::fgOrange(std::to_string(prsa.channels)));

      LOG(Logger::Priority::INFO, tab(4) + LogColor::fgBlue("Channel Layout") +
                                      colon +
                                      LogColor::fgOrange(prsa.channel_layout));

      LOG(Logger::Priority::INFO,
          tab(4) + LogColor::fgBlue("Sample Rate") + colon +
              LogColor::fgOrange(std::to_string(prsa.sample_rate)));

      LOG(Logger::Priority::INFO,
          tab(4) + LogColor::fgBlue("Bit Rate") + colon +
              LogColor::fgOrange(std::to_string(prsa.tags.BPS)));
    }

    LOG(Logger::Priority::INFO, "");
    // subtitle bloc
    LOG(Logger::Priority::INFO,
        tab(2) + ob + LogColor::fgWhite("Subtitle Streams") + cb);

    // iterate subtitle streams
    for (int i = 0; i < media->probeResult->subtitleStreams.size(); i++) {
      ProbeResultStreamSubtitle prss = media->probeResult->subtitleStreams[i];

      // subtitle lang bloc
      LOG(Logger::Priority::INFO, tab(3) + ob + std::to_string(i) + cb + " " +
                                      LogColor::fgOrange(prss.tags.language));

      LOG(Logger::Priority::INFO,
          tab(4) + LogColor::fgBlue("Title") + colon +
              LogColor::fgOrange(prss.tags.title.empty() ? "None"
                                                         : prss.tags.title));

      LOG(Logger::Priority::INFO, tab(4) + LogColor::fgBlue("Codec") + colon +
                                      LogColor::fgOrange(prss.codec_name));
    }

    LOG(Logger::Priority::INFO, "");
    // attachment bloc
    LOG(Logger::Priority::INFO,
        tab(2) + ob + LogColor::fgWhite("Attachment Streams") + cb);

    // iterate subtitle streams
    for (int i = 0; i < media->probeResult->attachmentStreams.size(); i++) {
      ProbeResultStreamAttachment prsa =
          media->probeResult->attachmentStreams[i];

      LOG(Logger::Priority::INFO, tab(3) + ob + std::to_string(i) + cb + " " +
                                      LogColor::fgOrange(prsa.tags.filename));

      LOG(Logger::Priority::INFO, tab(4) + LogColor::fgBlue("Mimetype") +
                                      colon +
                                      LogColor::fgOrange(prsa.tags.mimetype));

      LOG(Logger::Priority::INFO,
          tab(4) + LogColor::fgBlue("Codec") + colon +
              LogColor::fgOrange(prsa.codec_name.empty() ? "None"
                                                         : prsa.codec_name));
    }

    media_t_queue.push(media);
  }

  // assign the temp media queue to the child's pending queue
  child->pending = media_t_queue;
}

void ChildDisplay::printJSON() {}
