#ifndef F44FAB5C_C156_4991_9EF4_5C660F589437
#define F44FAB5C_C156_4991_9EF4_5C660F589437

#include "nlohmann/json.hpp"
#include "src/program/generics/JSONSerializable.h"
#include <string>

struct FileVideoInfo : JSONSerializable {

  float fps; /// @brief FPS of the media.

  unsigned long long totalFrames; /// @brief Total frames of the media.
  unsigned long long size;
  unsigned long long new_size;

  int width;  /// @brief Width of the media.
  int height; /// @brief Height of the media.
  int crf;    /// @brief CRF of the media.

  /// @brief The desired resolution of the media for conversion.
  std::string convertedResolution;
  /// @brief The desired subtitle provider of the media for conversion.
  std::string subtitleProvider;
  /// @brief The desired height of the media for conversion.
  std::string convertedHeight;
  /// @brief The desired width of the media for conversion.
  std::string convertedWidth;
  /// @brief The desired ratio of the media for conversion.
  std::string ratio;
  /// @brief The desired crop of the media for conversion.
  std::string crop;

  void fromJSON(const nlohmann::json &json) override {
    fps = json["fps"];
    totalFrames = json["totalFrames"];
    size = json["size"];
    new_size = json["new_size"];
    width = json["width"];
    height = json["height"];
    crf = json["crf"];
    convertedResolution = json["convertedResolution"];
    subtitleProvider = json["subtitleProvider"];
    convertedHeight = json["convertedHeight"];
    convertedWidth = json["convertedWidth"];
    ratio = json["ratio"];
    crop = json["crop"];
  }

  nlohmann::json toJSON() override {
    nlohmann::json json;
    json["fps"] = fps;
    json["totalFrames"] = totalFrames;
    json["size"] = size;
    json["new_size"] = new_size;
    json["width"] = width;
    json["height"] = height;
    json["crf"] = crf;
    json["convertedResolution"] = convertedResolution;
    json["subtitleProvider"] = subtitleProvider;
    json["convertedHeight"] = convertedHeight;
    json["convertedWidth"] = convertedWidth;
    json["ratio"] = ratio;
    json["crop"] = crop;
    return json;
  }
};

#endif /* F44FAB5C_C156_4991_9EF4_5C660F589437 */
