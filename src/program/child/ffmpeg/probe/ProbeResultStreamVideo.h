#if !defined(PROBE_RESULT_STREAM_VIDEO)
#define PROBE_RESULT_STREAM_VIDEO

#include <nlohmann/json_fwd.hpp>
#include <string>

#include "ProbeResultStream.h"

class ProbeResultStreamVideo : public ProbeResultStream {
 public:
  /** This constructor should never be called. */
  ProbeResultStreamVideo();

  /**
   * @brief Construct a new Probe Result Stream Video object
   *
   * @param JSON - The FFProbe video stream JSON.
   */
  ProbeResultStreamVideo(const nlohmann::json JSON);
  ~ProbeResultStreamVideo();

  /// @brief codec profile
  std::string profile;
  /// @brief width of the media
  int width;
  /// @brief height of the media
  int height;
  /// @brief ???
  int coded_width;
  /// @brief ???
  int coded_height;
  /// @brief if closed captions are present
  int closed_captions;
  /// @brief if the stream uses film grain
  int film_grain;
  /// @brief number of b frames
  int has_b_frames;
  /// @brief ???
  std::string sample_aspect_ratio;
  /// @brief media display aspect ratio
  std::string display_aspect_ratio;
  /// @brief pixel format
  std::string pix_fmt;
  /// @brief media level ???
  int level;
  /// @brief media color range
  std::string color_range;
  /// @brief media color space
  std::string color_space;
  /// @brief media color transfer
  std::string color_transfer;
  /// @brief media color primaries
  std::string color_primaries;
  /// @brief media chroma location
  std::string chroma_location;
  /// @brief media field order
  std::string field_order;
  /// @brief ???
  int refs;
  /// @brief is audio video codec
  int is_avc;
  /// @brief ???
  std::string nal_length_size;
  /// @brief ???
  std::string bits_per_raw_sample;
};
#endif  // PROBE_RESULT_STREAM_VIDEO
