/*********************************************************************
 * @file   ProbeResultStreamDisposition.h
 * @brief  Probe Result Stream Disposition class header.
 *
 * @author boooiil
 * @date   January 2024
 *********************************************************************/

#if !defined(PROBE_RESULT_STREAM_DISPOSITION)
#define PROBE_RESULT_STREAM_DISPOSITION

#include <nlohmann/json_fwd.hpp>

/**
 * @brief A class to represent the stream disposition of a FFProbe stream.
 */
class ProbeResultStreamDisposition {
public:
  /**
   * @brief Construct a new Probe Result Stream Disposition object
   *
   * @param[in] JSON - The FFProbe stream disposition JSON.
   */
  ProbeResultStreamDisposition(const nlohmann::json JSON);
  ~ProbeResultStreamDisposition() = default;

  /// @brief if this stream is the default stream
  int default_;
  /// @brief if this stream is a dub
  int dub;
  /// @brief if this stream is an original
  int original;
  /// @brief if this stream is a comment
  int comment;
  /// @brief if this stream is a lyrics
  int lyrics;
  /// @brief if this stream is karaoke
  int karaoke;
  /// @brief if this stream is forced
  int forced;
  /// @brief if this stream is hearing impaired
  int hearing_impaired;
  /// @brief if this stream is visual impaired
  int visual_impaired;
  /// @brief if this stream is clean effects
  int clean_effects;
  /// @brief if this stream is attached pic
  int attached_pic;
  /// @brief if this stream is timed thumbnails
  int timed_thumbnails;
  /// @brief if this stream is captions
  int captions;
  /// @brief if this stream is descriptions
  int descriptions;
  /// @brief if this stream is metadata
  int metadata;
  /// @brief if this stream is dependent
  int dependent;
  /// @brief if this stream is still image
  int still_image;
};

#endif // PROBE_RESULT_STREAM_DISPOSITION
