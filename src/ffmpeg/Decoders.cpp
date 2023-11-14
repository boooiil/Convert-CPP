#include "Decoders.h"

std::unordered_map<Decoders::Codec, std::string> Decoders::codecMap = {
    {Decoders::AV1_CUVID, "av1_cuvid"},   {Decoders::AV1_QSV, "av1_qsv"},
    {Decoders::H264_CUVID, "h264_cuvid"}, {Decoders::H264_QSV, "h264_qsv"},
    {Decoders::HEVC_CUVID, "hevc_cuvid"}, {Decoders::HEVC_QSV, "hevc_qsv"}};

Decoders::Decoders() {}
Decoders::~Decoders() {}

/**
 * @brief Get the value of the codec to be used in FFMpeg arguments.
 *
 * @param codec The decoder codec.
 * @return std::string The FFMpeg value of the codec.
 */
std::string Decoders::getValue(Decoders::Codec codec) {
  return codecMap.at(codec);
}