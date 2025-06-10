#ifndef TUNES_H
#define TUNES_H

#include <string>
#include <vector>

#include "StringEnumDataHolder.h"

enum class Tunes {
  INVALID = 0,

  DEFAULT,
  FAST,
  BALANCED,
  FILM,
  ANIMATION,
  GRAIN,
  STILL_IMAGE,
  PSNR,
  SSIM,
  FAST_DECODE,
  LOW_LATENCY,
  ZERO_LATENCY,
  TEXTURE_COMPRESSION,
  PROXY,
  HIGH_QUALITY,
  LOW_QUALITY,
  ZERO_LATENCY_LOW_LATENCY,
  LOSSLESS
  /*public:
   static const Tunes DEFAULT;
   static const Tunes FILM;
   static const Tunes ANIMATION;
   static const Tunes GRAIN;
   static const Tunes STILL_IMAGE;
   static const Tunes PSNR;
   static const Tunes SSIM;
   static const Tunes FAST_DECODE;
   static const Tunes ZERO_LATENCY;
   static const Tunes TEXTURE_COMPRESSION;
   static const Tunes PROXY;
   static const Tunes HIGH_QUALITY;
   static const Tunes LOW_QUALITY;
   static const Tunes ZERO_LATENCY_LOW_LATENCY;
   static const Tunes LOSSLESS;

   static const std::vector<const Tunes*> _all() {
     return {&DEFAULT,      &FILM,
             &ANIMATION,    &GRAIN,
             &STILL_IMAGE,  &PSNR,
             &SSIM,         &FAST_DECODE,
             &ZERO_LATENCY, &TEXTURE_COMPRESSION,
             &PROXY,        &HIGH_QUALITY,
             &LOW_QUALITY,  &ZERO_LATENCY_LOW_LATENCY,
             &LOSSLESS};
   }

   static const Tunes getKey(std::string value) {
     for (auto& item : _all()) {
       if (item->getName() == value) {
         return *item;
       }
     }
     return DEFAULT;
   }*/
};

#endif  // TUNES_H