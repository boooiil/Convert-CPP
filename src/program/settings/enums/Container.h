#ifndef CONTAINER_H
#define CONTAINER_H

#include <string>
#include <vector>

#include "StringEnumDataHolder.h"

enum class Container {
  INVALID = 0,

  AVI,
  MKV,
  MOV,
  MP3,
  MP4
  //public:
  // static const StringEnumDataHolder<Container> INVALID;
  // static const StringEnumDataHolder<Container> AVI;  // mpeg4, mp3, none
  // static const StringEnumDataHolder<Container> MKV;  // h264, vorbis, ass
  // static const StringEnumDataHolder<Container> MOV;  // h264, aac, none
  // static const StringEnumDataHolder<Container> MP3;  // png, mp3, none
  // static const StringEnumDataHolder<Container> MP4;  // h264, aac, none

  // static const std::vector<const StringEnumDataHolder<Container>*> _all() {
  //   return {&INVALID, &AVI, &MKV, &MOV, &MP3, &MP4};
  // };

  // static const StringEnumDataHolder<Container> getKey(std::string value) {
  //   for (auto& item : _all()) {
  //     if (item->getName() == value) {
  //       return *item;
  //     }
  //   }
  //   return INVALID;
  // }
};

#endif  // CONTAINER_H