/*********************************************************************
 * @file   Decoders.h
 * @brief  Decoders class header.
 *
 * @author boooiil
 * @date   January 2024
 *********************************************************************/

#if !defined(DECODERS)
#define DECODERS

#include <string>
#include <unordered_map>
#include <vector>

#include "StringEnumDataHolder.h"

 /**
  * @brief List of decoders supported by the application.
  *
  */
enum class Decoders {
  INVALID = 0,

  NONE,
  AV1_CUVID,
  AV1_QSV,
  H264_CUVID,
  H264_QSV,
  HEVC_CUVID,
  HEVC_QSV

  //public:
  // static const StringEnumDataHolder<Decoders> NONE;
  // static const StringEnumDataHolder<Decoders> AV1_CUVID;
  // static const StringEnumDataHolder<Decoders> AV1_QSV;
  // static const StringEnumDataHolder<Decoders> H264_CUVID;
  // static const StringEnumDataHolder<Decoders> H264_QSV;
  // static const StringEnumDataHolder<Decoders> HEVC_CUVID;
  // static const StringEnumDataHolder<Decoders> HEVC_QSV;

  // static const std::vector<const StringEnumDataHolder<Decoders>*> _all() {
  //   return {&NONE,     &AV1_CUVID,  &AV1_QSV, &H264_CUVID,
  //           &H264_QSV, &HEVC_CUVID, &HEVC_QSV};
  // }

  // static const StringEnumDataHolder<Decoders> getKey(std::string value) {
  //   for (auto& item : _all()) {
  //     if (item->getName() == value) {
  //       return *item;
  //     }
  //   }
  //   return NONE;
  // }
};

#endif  // DECODERS
