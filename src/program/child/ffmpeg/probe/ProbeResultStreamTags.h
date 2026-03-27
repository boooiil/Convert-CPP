/*********************************************************************
 * @file   ProbeResultStreamTags.h
 * @brief  Probe Result Stream Tags class header.
 *
 * @author boooiil
 * @date   January 2024
 *********************************************************************/

#if !defined(PROBE_RESULT_STREAM_TAGS)
#define PROBE_RESULT_STREAM_TAGS

#include <nlohmann/json_fwd.hpp>
#include <string>

/**
 * @brief This class represents the tags of a stream in a FFProbe result.
 */
class ProbeResultStreamTags {
public:
  /** This constructor should never be called. */
  ProbeResultStreamTags();

  /**
   * @brief Construct a new Probe Result Stream Tags object
   *
   * @param[in] JSON - The FFProbe tag JSON.
   */
  ProbeResultStreamTags(const nlohmann::json JSON);
  ~ProbeResultStreamTags();

  /// @brief language of the stream
  std::string language;
  /// @brief title of the stream
  std::string title;
  /// @brief bits per second
  int BPS;
  /// @brief number of frames in the stream
  std::string NUMBER_OF_FRAMES;
  /// @brief number of bytes of the stream
  std::string NUMBER_OF_BYTES;
  /// @brief name of the statistics writer
  std::string _STATISTICS_WRITING_APP;
  /// @brief date when the tags were written
  std::string _STATISTICS_WRITING_DATE_UTC;
  /// @brief list of properties that are included
  std::string _STATISTICS_TAGS;
  /// @brief name of the encoder
  std::string ENCODER;
  /// @brief HH:MM:SS.ms
  std::string DURATION;
};

#endif // PROBE_RESULT_STREAM_TAGS
