#include "./Display.h"

#include <math.h>

#include "../utils/StringUtils.h"
#include "Debug.h"

Display::Display(Container& container) : container(container) {}

Display::~Display() {}

void Display::print() {
  int bufferLen =
      this->container.converting.size() + this->container.pending.size() + 1;

  std::string ob = LogColor::fgGray("[");
  std::string cb = LogColor::fgGray("]");

  std::string time = ob + LogColor::fgCyan("TIME") + cb +
                     LogColor::fgGray(" 00:00:00 00-00-0000");
  std::string encoder =
      ob + LogColor::fgCyan("TARGET ENC") + cb + " " +
      LogColor::fgGray(this->container.appEncodingDecision.wantedEncoder);
  std::string runningEncoder =
      ob + LogColor::fgCyan("ENC") + cb + " " +
      LogColor::fgGray(this->container.appEncodingDecision.runningEncoder);
  std::string runningDecoder =
      ob + LogColor::fgCyan("DEC") + cb + " " +
      LogColor::fgGray(this->container.appEncodingDecision.runningDecoder);
  std::string resolution =
      ob + LogColor::fgCyan("RES") + cb + " " +
      LogColor::fgGray(this->container.appEncodingDecision.quality);
  std::string tune = ob + LogColor::fgCyan("TUNE") + cb + " " +
                     LogColor::fgGray(this->container.appEncodingDecision.tune);
  std::string amount =
      ob + LogColor::fgCyan("AMOUNT") + cb + " " +
      LogColor::fgGray(std::to_string(container.appEncodingDecision.amount));

  std::string constrain = ob + LogColor::fgRed("CONSTRAIN") + cb;
  std::string debug = ob + LogColor::fgRed("DEBUG") + cb;
  std::string crop = ob + LogColor::fgRed("CROP") + cb;

  std::string header = time + " " + encoder + " " + runningEncoder + " " +
                       runningDecoder + " " + resolution + " " + tune + " " +
                       amount;

  if (this->container.appEncodingDecision.useConstrain) {
    header += " " + constrain;
  }

  if (Debug::toggle) {
    header += " " + debug;
  }

  if (this->container.appEncodingDecision.crop) {
    header += " " + crop;
  }

  // clear console?
  this->container.log.send({"\x1B[2J\x1B[H"});
  this->container.log.sendBuffer(bufferLen, header);

  std::queue<Media> t_queue;

  while (!this->container.converting.empty()) {
    Media media = this->container.converting.front();
    this->container.converting.pop();

    // create a time util to get this
    std::string started = ob + LogColor::fgCyan("START") + cb + "00:00:00";
    // create a time util to get this
    std::string eta = ob + LogColor::fgCyan("ETA") + cb + "00:00:00";

    int completedFrames = media.working.completedFrames;
    int totalFrames = media.video.totalFrames;

    float crf = media.working.quality;
    float v_crf = media.video.crf;

    float workingFPS = media.working.fps;
    float videoFPS = media.video.fps;

    std::string fileName = ob + LogColor::fgCyan("FILE") + cb + " " +
                           LogColor::fgGray(StringUtils::truncateString(
                               media.file.modifiedFileName));

    std::string activity = ob + LogColor::fgCyan("ACT") + cb + " " +
                           Activity::getValue(media.activity);

    std::string percent =
        ob + LogColor::fgCyan("PROG") + cb + " " +
        std::to_string(static_cast<int>((static_cast<float>(completedFrames) /
                                         static_cast<float>(totalFrames)) *
                                        100)) +
        "%";

    std::string cq = ob + LogColor::fgCyan("QUAL") + cb + " " +
                     std::to_string(static_cast<int>(v_crf / crf) * 100);
    std::string speed =
        ob + LogColor::fgCyan("SPEED") + cb + " " +
        std::to_string(trunc((workingFPS / videoFPS) * 100) / 100);

    std::string bitrate = ob + LogColor::fgCyan("BITRATE") + cb + " " +
                          std::to_string(media.working.bitrate) + "kb/s";

    this->container.log.sendBuffer(
        bufferLen, fileName + " " + activity + " " + started + " " + percent +
                       " " + cq + " " + bitrate + " " + speed + " " + eta);

    t_queue.push(media);
  }

  this->container.converting = t_queue;
  t_queue = std::queue<Media>();

  while (!this->container.pending.empty()) {
    Media media = this->container.pending.front();
    this->container.pending.pop();

    std::string fileName = ob + LogColor::fgCyan("FILE") + cb + " " +
                           LogColor::fgGray(StringUtils::truncateString(
                               media.file.modifiedFileName));

    std::string activity = ob + LogColor::fgCyan("ACT") + cb + " " +
                           Activity::getValue(media.activity);

    if (media.activity == Activity::FINISHED) {
      std::string ended;
      std::string elapsed;
      std::string reduced;

      this->container.log.sendBuffer(bufferLen, fileName + " " + activity +
                                                    " " + reduced + " " +
                                                    ended + " " + elapsed);
    } else {
      this->container.log.sendBuffer(bufferLen, fileName + " " + activity);
    }

    t_queue.push(media);
  }
  this->container.pending = t_queue;
}
void Display::printDebug() {}