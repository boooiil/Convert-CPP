#include "MediaFormat.h"

#include <math.h>

#include <string>

MediaFormat::MediaFormat()
    : name(""), crop(""), scale(""), crf(0), bitrate(0), min(0), max(0),
      width(0), height(0) {}

MediaFormat::MediaFormat(std::string name, int crf, float bitrate, float min,
                         float max, int width, int height, std::string crop,
                         std::string scale)
    : name(name), crop(crop), scale(scale), crf(crf), bitrate(bitrate),
      min(min), max(max), width(width), height(height) {}

MediaFormat::~MediaFormat() {}

int MediaFormat::getGCD(int a, int b) {
  if (b == 0) {
    return a;
  }
  return getGCD(b, a % b);
}

int MediaFormat::getResolution(int width, int height, int newWidth) {
  int newHeight =
      static_cast<int>(ceil(static_cast<double>(height) / width * newWidth));
  return newHeight % 2 == 0 ? newHeight : newHeight + 1;
}

void MediaFormat::fromCustom(int n_height) {
  this->name = "custom";
  this->height = n_height % 2 ? n_height++ : n_height;
  this->width = static_cast<int>(ceil(this->height * 1.777777777777778));

  if (this->width % 2) {
    this->width++;
  }

  int cropHeight = static_cast<int>(ceil(this->width / 2.4));

  if (cropHeight % 2) {
    cropHeight++;
  }

  this->crop = std::to_string(this->width) + ":" + std::to_string(cropHeight);
  this->scale =
      std::to_string(this->width) + ":" + std::to_string(this->height);
};