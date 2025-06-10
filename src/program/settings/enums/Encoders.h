/*********************************************************************
 * @file   Encoders.h
 * @brief  Enum of encoders and utility functions.
 *
 * @author boooiil
 * @date   January 2024
 *********************************************************************/
#if !defined(ENCODERS)
#define ENCODERS

#include <string>
#include <unordered_map>
#include <vector>

/**
 * @brief Class for encoders.
 *
 */
enum class Encoders {
  INVALID = 0,

  H264,
  H264_AMF,
  H264_NVENC,
  H264_QSV,

  HEVC,
  HEVC_AMF,
  HEVC_NVENC,
  HEVC_QSV,

  AV1,
  AV1_AMF,
  AV1_NVENC,
  AV1_QSV
  // public:
  //  static const Encoders INVALID;

  // static const Encoders H264;
  // static const Encoders H264_AMF;
  // static const Encoders H264_NVENC;
  // static const Encoders H264_QSV;

  // static const Encoders HEVC;
  // static const Encoders HEVC_AMF;
  // static const Encoders HEVC_NVENC;
  // static const Encoders HEVC_QSV;

  // static const Encoders AV1;
  // static const Encoders AV1_AMF;
  // static const Encoders AV1_NVENC;
  // static const Encoders AV1_QSV;

  // static const std::vector<Encoders> _all() {
  //   return {H264,      H264_AMF,   H264_NVENC, H264_QSV, HEVC,
  //           HEVC_AMF,  HEVC_NVENC, HEVC_QSV,   AV1,      AV1_AMF,
  //           AV1_NVENC, AV1_QSV

  //   };
  // }

  // /**
  //  * @brief Get the codec from a string.
  //  *
  //  * @param[in] codec - Desired codec.
  //  *
  //  * @return Codec enum.
  //  */
  // static const Encoders getKey(std::string value) {
  //   for (auto& item : _all()) {
  //     if (item.getName() == value) {
  //       return item;
  //     }
  //   }
  //   return INVALID;
  // }

  // /**
  //  * @brief Check if the codec is H264.
  //  *
  //  * @param[in] codec - Desired codec.
  //  *
  //  * @return True if the codec is H264.
  //  */
  // static bool isH264(Encoders codec);

  // /**
  //  * @brief Check if the codec is HEVC.
  //  *
  //  * @param[in] codec - Desired codec.
  //  *
  //  * @return True if the codec is HEVC.
  //  */
  // static bool isHEVC(Encoders codec);

  // /**
  //  * @brief Check if the codec is AV1.
  //  *
  //  * @param[in] codec - Desired codec.
  //  *
  //  * @return True if the codec is AV1.
  //  */
  // static bool isAV1(Encoders codec);

  // /**
  //  * @brief Check if the codec is a hardware encoder.
  //  *
  //  * @param[in] codec - Desired codec.
  //  *
  //  * @return True if the codec is a hardware encoder.
  //  */
  // static bool isHardwareEncoder(Encoders codec);
};

#endif  // ENCODERS
