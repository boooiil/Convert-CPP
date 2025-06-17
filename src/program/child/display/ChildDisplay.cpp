#include "ChildDisplay.h"

#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <queue>
#include <string>

#include "../../../utils/NumberUtils.h"
#include "../../../utils/StringUtils.h"
#include "../../../utils/TimeUtils.h"
#include "../../../utils/logging/LogColor.h"
#include "../../../utils/logging/Logger.h"
#include "../../Program.h"
#include "../../generics/GenericRunner.h"
#include "../../settings/Settings.h"
#include "../../settings/arguments/ArgumentRegistry.h"
#include "../../settings/arguments/EnumArgument.h"
#include "../../settings/arguments/FlagArgument.h"
#include "../../settings/arguments/IntegerArgument.h"
#include "../../settings/arguments/video/Quality.h"
#include "../../settings/enums/Encoders.h"
#include "../../settings/enums/EnumToStringFactory.h"
#include "../../settings/enums/LoggingOptions.h"
#include "../../ticker/NTicker.h"
#include "../Child.h"
#include "../ffmpeg/probe/ProbeResultStreamAudio.h"
#include "../ffmpeg/probe/ProbeResultStreamSubtitle.h"
#include "../ffmpeg/probe/ProbeResultStreamVideo.h"
#include "../media/Media.h"

// TODO: appending to string rebuilds each time
//  CONT: we should maybe do a sstream

void ChildDisplay::print() {
  Child& child =
      *Program::ticker->getRunner<NTicker>()->runner->getRunner<Child>();

  ChildOptions& childOptions = *Program::settings->childOptionsMap[child.id];
  ArgumentRegistry& argumentRegistry = *childOptions.argumentRegistry;

  std::string sendStr = "";

  // int bufferLen = static_cast<int>(child.converting.size()) +
  //                 static_cast<int>(child.pending.size()) + 1;

  std::string ob = LogColor::fgGray("[");
  std::string cb = LogColor::fgGray("]");

  std::string time = ob + LogColor::fgCyan("TIME") + cb + " " +
                     TimeUtils::dateFormat(TimeUtils::getEpoch());
  std::string encoder =
      ob + LogColor::fgCyan("TARGET ENC") + cb + " " +
      LogColor::fgGray(
          EnumToStringFactory::get(
              argumentRegistry.get_t<BaseArgument<Encoders>>(Command::ENCODER)
                  ->get())
              .getName());
  std::string runningEncoder =
      ob + LogColor::fgCyan("ENC") + cb + " " +
      LogColor::fgGray(
          EnumToStringFactory::get(childOptions.runningEncoder).getName());
  std::string runningDecoder =
      ob + LogColor::fgCyan("ACC") + cb + " " +
      LogColor::fgGray(
          EnumToStringFactory::get(childOptions.runningHWAccel).getName());
  std::string resolution =
      ob + LogColor::fgCyan("RES") + cb + " " +
      LogColor::fgGray(
          argumentRegistry.get_t<Quality>(Command::QUALITY)->get().name);
  std::string tune =
      ob + LogColor::fgCyan("TUNE") + cb + " " +
      LogColor::fgGray(
          EnumToStringFactory::get(
              argumentRegistry.get_t<BaseArgument<Tunes>>(Command::TUNE)->get())
              .getName());
  std::string amount =
      ob + LogColor::fgCyan("AMOUNT") + cb + " " +
      LogColor::fgGray(
          argumentRegistry.get_t<IntegerArgument>(Command::AMOUNT)->toString());

  // std::string a = ArgumentRegistry::get_t<IntegerArgument>("-a");

  std::string constrain = ob + LogColor::fgRed("CONSTRAIN") + cb;
  std::string debug = ob + LogColor::fgRed("DEBUG") + cb;
  std::string crop = ob + LogColor::fgRed("CROP") + cb;

  std::string header = time + " " + encoder + " " + runningEncoder + " " +
                       runningDecoder + " " + resolution + " " + tune + " " +
                       amount;

  if (argumentRegistry.get_t<FlagArgument>(Command::CONSTRAIN)->get()) {
    header += " " + constrain;
  }

  if (Program::settings->programOptions->argumentRegistry
          ->get_t<EnumArgument<LoggingOptions>>(Command::LOGGINGOPTIONS)
          ->get() == LoggingOptions::DEBUG) {
    header += " " + debug;
  }

  if (argumentRegistry.get_t<FlagArgument>(Command::CROP)->get()) {
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

  std::queue<Media*> t_queue;

  while (!child.converting.empty()) {
    Media* media = child.converting.front();
    child.converting.pop();

    float mediaFPS = media->working->fps > 0 ? media->working->fps : 1;
    double totalFrames = static_cast<double>(media->video->totalFrames);
    double completedFrames =
        static_cast<double>(media->working->completedFrames);

    int eta_result = static_cast<int>(
        ceil((totalFrames - completedFrames) / mediaFPS) * 1000);

    int percent_result =
        static_cast<int>(std::round((completedFrames / totalFrames) * 100));

    // create a time util to get this
    std::string started = ob + LogColor::fgCyan("START") + cb + " " +
                          TimeUtils::timeFormat(media->started);
    // create a time util to get this
    std::string eta = ob + LogColor::fgCyan("ETA") + cb + " " +
                      TimeUtils::durationFormat(eta_result);

    float crf = media->working->quality;
    int v_crf = media->video->crf;

    float workingFPS = media->working->fps;
    float videoFPS = media->video->fps;

    std::string fileName = ob + LogColor::fgCyan("FILE") + cb + " " +
                           LogColor::fgGray(StringUtils::truncateString(
                               media->file->conversionName, 25));

    std::string activity =
        ob + LogColor::fgCyan("ACT") + cb + " " +
        EnumToStringFactory::get(media->getActivity()).getName();

    std::string percent = ob + LogColor::fgCyan("PROG") + cb + " " +
                          std::to_string(percent_result) + "%";

    std::string cq = ob + LogColor::fgCyan("QUAL") + cb + " " +
                     NumberUtils::formatNumber((v_crf / crf) * 100, 2) + "%";
    std::string speed = ob + LogColor::fgCyan("SPEED") + cb + " " +
                        NumberUtils::formatNumber(workingFPS / videoFPS, 2);

    std::string bitrate =
        ob + LogColor::fgCyan("BITRATE") + cb + " " +
        NumberUtils::formatNumber(media->working->bitrate, 2) + "kb/s";

    sendStr += fileName + " " + activity + " " + started + " " + percent + " " +
               cq + " " + bitrate + " " + speed + " " + eta + "\n";
    // Program::log->sendBuffer(
    //     bufferLen, fileName + " " + activity + " " + started + " " + percent
    //     + " " + cq + " " + bitrate + " " + speed + " " + eta);

    t_queue.push(media);
  }

  child.converting = t_queue;
  t_queue = std::queue<Media*>();

  while (!child.pending.empty()) {
    Media* media = child.pending.front();
    child.pending.pop();

    std::string fileName = ob + LogColor::fgCyan("FILE") + cb + " " +
                           LogColor::fgGray(StringUtils::truncateString(
                               media->file->conversionName, 25));

    std::string activity =
        ob + LogColor::fgCyan("ACT") + cb + " " +
        EnumToStringFactory::get(media->getActivity()).getName();

    if (media->hasFinished()) {
      double currSize = static_cast<double>(media->file->size);
      double newSize = static_cast<double>(media->file->newSize);

      int calculatedSize =
          static_cast<int>(std::round(((currSize - newSize) / currSize) * 100));

      std::string ended = ob + LogColor::fgCyan("END") + cb + " " +
                          TimeUtils::timeFormat(media->ended);
      std::string elapsed =
          ob + LogColor::fgCyan("ELAPSED") + cb + " " +
          TimeUtils::durationFormat((media->ended - media->started) * 1000);

      std::string reduced = ob + LogColor::fgCyan("REDUCED") + cb + " " +
                            std::to_string(calculatedSize) + "%";

      sendStr += fileName + " " + activity + " " + reduced + " " + ended + " " +
                 elapsed + "\n";
      // Program::log->sendBuffer(bufferLen, fileName + " " + activity + " " +
      //                                         reduced + " " + ended + " " +
      //                                         elapsed);
    } else {
      sendStr += fileName + " " + activity + "\n";
      // Program::log->sendBuffer(bufferLen, fileName + " " + activity);
    }

    t_queue.push(media);
  }
  child.pending = t_queue;
  LOG(sendStr);
}

void ChildDisplay::printDebug(void) {
  // TODO: finish
}

void ChildDisplay::printInformation(void) {
  LOG_DEBUG("Printing Information");

  NTicker* ticker = Program::ticker->getRunner<NTicker>();
  Child* runner = ticker->runner->getRunner<Child>();

  LOG(LogColor::fgBlack("Black"), LogColor::fgRed("Red"),
      LogColor::fgGreen("Green"), LogColor::fgGray("Gray"),
      LogColor::fgYellow("Yellow"), LogColor::fgBlue("Blue"),
      LogColor::fgOrange("Orange"), LogColor::fgMagenta("Magenta"),
      LogColor::fgCyan("Cyan"), LogColor::fgWhite("White"));

  if (runner->pending.empty()) {
    LOG(LogColor::fgRed("No media files found."));
    return;
  }

  std::queue<Media*> t_queue;

  while (!runner->pending.empty()) {
    Media* media = runner->pending.front();
    runner->pending.pop();

    media->doStatistics();
    media->buildFFmpegArguments(false);

    std::string nl = "\n";
    std::string ob = LogColor::fgGray("[");
    std::string cb = LogColor::fgGray("]");
    std::string colon = LogColor::fgGray(": ");

    LOG(nl + ob + LogColor::fgRed(media->file->originalFileNameExt) + cb + nl);

    LOG(LogColor::fgWhite("  " + ob + "Format") + cb);

    LOG(LogColor::bgBlue("    Duration") + colon +
        LogColor::bgOrange(media->probeResult->format.duration));

    LOG(LogColor::bgBlue("    Format: ") +
        LogColor::bgOrange(media->probeResult->format.format_name));

    LOG(LogColor::bgBlue("    Bit Rate: ") +
        LogColor::bgOrange(media->probeResult->format.bit_rate));

    LOG(LogColor::bgBlue("    Size: ") +
        LogColor::bgOrange(media->probeResult->format.size));

    LOG(LogColor::bgBlue("    Stream Count: ") +
        LogColor::bgOrange(
            std::to_string(media->probeResult->format.nb_streams)));

    LOG(LogColor::fgWhite(nl + "  " + ob + "Video Streams") + cb);

    // video streams
    for (int i = 0; i < media->probeResult->videoStreams.size(); i++) {
      ProbeResultStreamVideo prsv = media->probeResult->videoStreams[i];

      LOG("    " + ob + std::to_string(i) + cb + " " +
          LogColor::fgOrange(prsv.codec_name));

      LOG(LogColor::fgBlue("      Profile") + colon +
          LogColor::fgOrange(prsv.profile.empty() ? "None" : prsv.profile));

      LOG(LogColor::fgBlue("      Res") + colon +
          LogColor::fgOrange(std::to_string(prsv.width) + "x" +
                             std::to_string(prsv.height)) +
          " (" + LogColor::fgOrange(prsv.display_aspect_ratio) + ")");

      LOG(LogColor::fgBlue("      Listed Duration") + colon +
          LogColor::fgOrange(prsv.tags.DURATION.empty() ? "None"
                                                        : prsv.tags.DURATION));
    }

    LOG(LogColor::fgWhite(nl + "  " + ob + "Audio Streams") + cb);

    // audio streams
    for (int i = 0; i < media->probeResult->audioStreams.size(); i++) {
      ProbeResultStreamAudio prsa = media->probeResult->audioStreams[i];

      LOG("    " + ob + std::to_string(i) + cb + " " +
          LogColor::fgOrange(prsa.tags.language));

      LOG(LogColor::fgBlue("      Title") + colon +
          LogColor::fgOrange(prsa.tags.title.empty() ? "None"
                                                     : prsa.tags.title));

      LOG(LogColor::fgBlue("      Codec") + colon +
          LogColor::fgOrange(prsa.codec_name));

      LOG(LogColor::fgBlue("      Channels") + colon +
          LogColor::fgOrange(std::to_string(prsa.channels)));

      LOG(LogColor::fgBlue("      Channel Layout") + colon +
          LogColor::fgOrange(prsa.channel_layout));

      LOG(LogColor::fgBlue("      Sample Rate") + colon +
          LogColor::fgOrange(prsa.sample_rate));

      LOG(LogColor::fgBlue("      Bit Rate") + colon +
          LogColor::fgOrange(prsa.tags.BPS));
    }

    LOG(LogColor::fgWhite(nl + "  " + ob + "Subtitle Streams") + cb);

    // subtitle streams
    for (int i = 0; i < media->probeResult->subtitleStreams.size(); i++) {
      ProbeResultStreamSubtitle prss = media->probeResult->subtitleStreams[i];

      LOG("    " + ob + std::to_string(i) + cb + " " +
          LogColor::fgOrange(prss.tags.language));

      LOG(LogColor::fgBlue("      Title") + colon +
          LogColor::fgOrange(prss.tags.title));

      LOG(LogColor::fgBlue("      Codec") + colon +
          LogColor::fgOrange(prss.codec_name));
    }

    t_queue.push(media);
  }

  runner->converting = t_queue;
}

void ChildDisplay::printJSON(void) {
  // clear console?
#ifdef _WIN32
  system("cls");
#else
  system("clear");
#endif
  // TODO: finish
  puts(Program::ticker->toJSON().dump().c_str());
}