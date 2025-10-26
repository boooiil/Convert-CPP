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
#include "../../settings/Settings.h"
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
//  CONT: we should do a sstream

// NOLINTBEGIN(modernize-return-braced-init-list)
inline auto tab(int spaces) -> std::string {
  return std::string(spaces * 2, ' ');
}
// NOLINTEND(modernize-return-braced-init-list)

void ChildDisplay::print() {
  Child &child =
      *Program::ticker->getRunner<NTicker>()->runner->getRunner<Child>();

  ChildOptions &childOptions = *Program::settings->childOptionsMap[child.id];
  ArgumentRegistry &argumentRegistry = *childOptions.argumentRegistry;

  std::string sendStr = "";

  // int bufferLen = static_cast<int>(child.converting.size()) +
  //                 static_cast<int>(child.pending.size()) + 1;

  std::string time = StringUtils::bracket(
      "TIME", TimeUtils::dateFormat(TimeUtils::getEpoch()), LogColor::fgGray,
      LogColor::fgCyan, LogColor::fgWhite);

  std::string encoder = StringUtils::bracket(
      "TARGET ENC",
      EnumToStringFactory::get(
          argumentRegistry.get_t<BaseArgument<Encoders>>(Command::ENCODER)
              ->get()));

  std::string runningEncoder = StringUtils::bracket(
      "ENC", EnumToStringFactory::get(childOptions.runningEncoder));

  std::string runningDecoder = StringUtils::bracket(
      "ACC", EnumToStringFactory::get(childOptions.runningHWAccel));

  std::string resolution = StringUtils::bracket(
      "RES", argumentRegistry.get_t<Quality>(Command::QUALITY)->get().name);

  std::string tune = StringUtils::bracket(
      "TUNE",
      EnumToStringFactory::get(
          argumentRegistry.get_t<BaseArgument<Tunes>>(Command::TUNE)->get()));

  std::string amount = StringUtils::bracket(
      "AMOUNT",
      argumentRegistry.get_t<IntegerArgument>(Command::AMOUNT)->toString());

  std::string container = StringUtils::bracket(
      "CONTAINER",
      EnumToStringFactory::get<Container>(
          argumentRegistry.get_t<EnumArgument<Container>>(Command::CONTAINER)
              ->get()));

  // std::string a = ArgumentRegistry::get_t<IntegerArgument>("-a");

  std::string constrain =
      StringUtils::bracket("CONSTRAIN", "", LogColor::fgGray, LogColor::fgRed);
  std::string debug =
      StringUtils::bracket("DEBUG", "", LogColor::fgGray, LogColor::fgRed);
  std::string crop =
      StringUtils::bracket("CROP", "", LogColor::fgGray, LogColor::fgRed);

  std::string header = time + " " + encoder + " " + runningEncoder + " " +
                       runningDecoder + " " + resolution + " " + tune + " " +
                       amount + " " + container;

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

  std::queue<Media *> t_queue;

  while (!child.converting.empty()) {
    Media *media = child.converting.front();
    child.converting.pop();

    float mediaFPS = media->working->fps > 0 ? media->working->fps : 1;
    auto totalFrames = static_cast<double>(media->video->totalFrames);
    auto completedFrames = static_cast<double>(media->working->completedFrames);

    int eta_result = static_cast<int>(
        ceil((totalFrames - completedFrames) / mediaFPS) * 1000);

    int percent_result =
        static_cast<int>(std::round((completedFrames / totalFrames) * 100));

    // create a time util to get this
    std::string started =
        StringUtils::bracket("STR", TimeUtils::timeFormat(media->started));

    // create a time util to get this
    std::string eta =
        StringUtils::bracket("ETA", TimeUtils::durationFormat(eta_result));

    float crf = media->working->quality;
    int v_crf = media->video->crf;

    float workingFPS = media->working->fps;
    float videoFPS = media->video->fps;

    std::string fileName = StringUtils::bracket(
        "FILE", StringUtils::truncateString(media->file->conversionName, 25));

    std::string activity = StringUtils::bracket(
        "ACT", EnumToStringFactory::get(media->getActivity()));

    std::string progress =
        StringUtils::bracket("PROG", std::to_string(percent_result) + "%");

    std::string cq = StringUtils::bracket(
        "QUAL", NumberUtils::formatNumber((v_crf / crf) * 100, 2) + "%");

    std::string speed = StringUtils::bracket(
        "SPEED", NumberUtils::formatNumber(workingFPS / videoFPS, 2));

    std::string bitrate = StringUtils::bracket(
        "BITRATE",
        NumberUtils::formatNumber(media->working->bitrate, 2) + "kb/s");

    sendStr += fileName + " " + activity + " " + started + " " + progress +
               " " + cq + " " + bitrate + " " + speed + " " + eta + "\n";
    // Program::log->sendBuffer(
    //     bufferLen, fileName + " " + activity + " " + started + " " + percent
    //     + " " + cq + " " + bitrate + " " + speed + " " + eta);

    t_queue.push(media);
  }

  child.converting = t_queue;
  t_queue = std::queue<Media *>();

  while (!child.pending.empty()) {
    Media *media = child.pending.front();
    child.pending.pop();

    std::string fileName = StringUtils::bracket(
        "FILE", StringUtils::truncateString(media->file->conversionName, 25));

    std::string activity = StringUtils::bracket(
        "ACT", EnumToStringFactory::get(media->getActivity()));

    if (media->hasFinished()) {
      auto currSize = static_cast<double>(media->file->size);
      auto newSize = static_cast<double>(media->file->newSize);

      int calculatedSize =
          static_cast<int>(std::round(((currSize - newSize) / currSize) * 100));

      std::string ended =
          StringUtils::bracket("END", TimeUtils::timeFormat(media->ended));

      std::string elapsed = StringUtils::bracket(
          "ELAPSED",
          TimeUtils::durationFormat((media->ended - media->started) * 1000));

      std::string reduced =
          StringUtils::bracket("REDUCED", std::to_string(calculatedSize) + "%");

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
  Child &child =
      *Program::ticker->getRunner<NTicker>()->runner->getRunner<Child>();

  std::queue<Media *> t_queue;

  while (!child.converting.empty()) {
    Media *media = child.converting.front();
    child.converting.pop();
    auto totalFrames = static_cast<double>(media->video->totalFrames);
    auto completedFrames = static_cast<double>(media->working->completedFrames);

    int percent_result =
        static_cast<int>(std::round((completedFrames / totalFrames) * 100));

    LOG_DEBUG("TOTAL_FRAMES:" + std::to_string(totalFrames));
    LOG_DEBUG("COMPLETED_FRAMES:" + std::to_string(completedFrames));
    LOG_DEBUG("PERCENT_RESULT:" + std::to_string(percent_result));

    t_queue.push(media);
  }

  child.converting = t_queue;
}

void ChildDisplay::printInformationTyped(NTicker *ticker, Child *child) {
  assert(ticker != nullptr);
  assert(child != nullptr);

  LOG_DEBUG("Printing Information");

  LOG(LogColor::fgBlack("Black"), LogColor::fgRed("Red"),
      LogColor::fgGreen("Green"), LogColor::fgGray("Gray"),
      LogColor::fgYellow("Yellow"), LogColor::fgBlue("Blue"),
      LogColor::fgOrange("Orange"), LogColor::fgMagenta("Magenta"),
      LogColor::fgCyan("Cyan"), LogColor::fgWhite("White"));

  if (child->pending.empty()) {
    LOG(LogColor::fgRed("No media files found."));
    return;
  }

  std::queue<Media *> media_t_queue;
  while (!child->pending.empty()) {
    ChildOptions &childOptions = *Program::settings->childOptionsMap[child->id];

    Media *media = child->pending.front();
    child->pending.pop();

    media->doStatistics();
    media->buildFFmpegArguments(false);

    std::string nl = "\n";
    std::string ob = LogColor::fgGray("[");
    std::string cb = LogColor::fgGray("]");
    std::string colon = LogColor::fgGray(": ");

    LOG();
    // parent bloc
    LOG(ob + "Parent - " + LogColor::fgGray(childOptions.CWD) + cb);
    LOG();
    // file bloc
    LOG(tab(1) + ob + LogColor::fgRed(media->file->originalFileNameExt) + cb);
    LOG();
    // format bloc
    LOG(tab(2) + ob + LogColor::fgWhite("Format") + cb);

    LOG(tab(3) + LogColor::bgBlue("Duration") + colon +
        LogColor::bgOrange(media->probeResult->format.duration));

    LOG(tab(3) + LogColor::bgBlue("Format") + colon +
        LogColor::bgOrange(media->probeResult->format.format_name));

    LOG(tab(3) + LogColor::bgBlue("Bit Rate") + colon +
        LogColor::bgOrange(media->probeResult->format.bit_rate));

    LOG(tab(3) + LogColor::bgBlue("Size") + colon +
        LogColor::bgOrange(media->probeResult->format.size));

    LOG(tab(3) + LogColor::bgBlue("Stream Count") + colon +
        LogColor::bgOrange(
            std::to_string(media->probeResult->format.nb_streams)));
    LOG();
    // video stream bloc
    LOG(tab(2) + ob + LogColor::fgWhite("Video Streams") + cb);

    // iterate video streams
    for (int i = 0; i < media->probeResult->videoStreams.size(); i++) {
      ProbeResultStreamVideo prsv = media->probeResult->videoStreams[i];

      // codec bloc
      LOG(tab(3) + ob + std::to_string(i) + cb + " " +
          LogColor::fgOrange(prsv.codec_name));

      LOG(tab(4) + LogColor::fgBlue("Profile") + colon +
          LogColor::fgOrange(prsv.profile.empty() ? "None" : prsv.profile));

      LOG(tab(4) + LogColor::fgBlue("Res") + colon +
          LogColor::fgOrange(std::to_string(prsv.width) + "x" +
                             std::to_string(prsv.height)) +
          " (" + LogColor::fgOrange(prsv.display_aspect_ratio) + ")");

      LOG(tab(4) + LogColor::fgBlue("Listed Duration") + colon +
          LogColor::fgOrange(prsv.tags.DURATION.empty() ? "None"
                                                        : prsv.tags.DURATION));
    }

    LOG();
    // audio streams bloc
    LOG(tab(2) + ob + LogColor::fgWhite("Audio Streams") + cb);

    // iterate audio streams
    for (int i = 0; i < media->probeResult->audioStreams.size(); i++) {
      ProbeResultStreamAudio prsa = media->probeResult->audioStreams[i];

      // audio lang bloc
      LOG(tab(3) + ob + std::to_string(i) + cb + " " +
          LogColor::fgOrange(prsa.tags.language));

      LOG(tab(4) + LogColor::fgBlue("Title") + colon +
          LogColor::fgOrange(prsa.tags.title.empty() ? "None"
                                                     : prsa.tags.title));

      LOG(tab(4) + LogColor::fgBlue("Codec") + colon +
          LogColor::fgOrange(prsa.codec_name));

      LOG(tab(4) + LogColor::fgBlue("Channels") + colon +
          LogColor::fgOrange(std::to_string(prsa.channels)));

      LOG(tab(4) + LogColor::fgBlue("Channel Layout") + colon +
          LogColor::fgOrange(prsa.channel_layout));

      LOG(tab(4) + LogColor::fgBlue("Sample Rate") + colon +
          LogColor::fgOrange(std::to_string(prsa.sample_rate)));

      LOG(tab(4) + LogColor::fgBlue("Bit Rate") + colon +
          LogColor::fgOrange(prsa.tags.BPS));
    }

    LOG();
    // subtitle bloc
    LOG(tab(2) + ob + LogColor::fgWhite("Subtitle Streams") + cb);

    // iterate subtitle streams
    for (int i = 0; i < media->probeResult->subtitleStreams.size(); i++) {
      ProbeResultStreamSubtitle prss = media->probeResult->subtitleStreams[i];

      // subtitle lang bloc
      LOG(tab(3) + ob + std::to_string(i) + cb + " " +
          LogColor::fgOrange(prss.tags.language));

      LOG(tab(4) + LogColor::fgBlue("Title") + colon +
          LogColor::fgOrange(prss.tags.title));

      LOG(tab(4) + LogColor::fgBlue("Codec") + colon +
          LogColor::fgOrange(prss.codec_name));
    }

    media_t_queue.push(media);
  }

  // assign the temp media queue to the child's pending queue
  child->pending = media_t_queue;
}

void ChildDisplay::printJSON() {
  // clear console?
#ifdef _WIN32
  system("cls");
#else
  system("clear");
#endif
  // TODO: finish
  puts(Program::ticker->toJSON().dump().c_str());
}
