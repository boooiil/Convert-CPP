#ifndef E9A4F937_1239_4C06_AC64_3F6A73BBE6FC
#define E9A4F937_1239_4C06_AC64_3F6A73BBE6FC

#include "nlohmann/json.hpp"
#include "src/program/generics/JSONSerializable.h"

struct FileProcessingInfo : JSONSerializable {
  float fps;     // processing fps
  float bitrate; // encoding bitrate
  float quality; // crf

  unsigned long long completedFrames; // completed frames

  void fromJSON(const nlohmann::json &json) override {
    fps = json["fps"];
    bitrate = json["bitrate"];
    quality = json["quality"];
    completedFrames = json["completedFrames"];
  }

  nlohmann::json toJSON() override {
    nlohmann::json json;
    json["fps"] = fps;
    json["bitrate"] = bitrate;
    json["quality"] = quality;
    json["completedFrames"] = completedFrames;
    return json;
  }
};

#endif /* E9A4F937_1239_4C06_AC64_3F6A73BBE6FC */
