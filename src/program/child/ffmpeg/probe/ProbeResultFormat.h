/*********************************************************************
 * @file   ProbeResultFormat.h
 * @brief  Probe Result Format header.
 *
 * @author boooiil
 * @date   January 2024
 *********************************************************************/

#if !defined(PROBE_RESULT_FORMAT)
#define PROBE_RESULT_FORMAT

#include <nlohmann/json_fwd.hpp>
#include <string>

#include "ProbeResultFormatTags.h"

/**
 * @brief The FFProbe format result.
 */
class ProbeResultFormat {
 public:
  /** This constructor should never be called. */
  ProbeResultFormat();

  /**
   * @brief Construct a new Probe Result Format object
   *
   * @param[in] JSON - The FFProbe result JSON.
   */
  ProbeResultFormat(const nlohmann::json JSON);
  ~ProbeResultFormat();

  /// @brief name of the file in current path ./{name}
  std::string filename;
  /// @brief ???
  int nb_streams;
  /// @brief ???
  int nb_programs;
  /// @brief container format
  std::string format_name;
  /// @brief container format long name
  std::string format_long_name;
  /// @brief stream start time
  std::string start_time;
  /// @brief stream duration
  std::string duration;
  /// @brief stream bitrate in bits
  std::string bit_rate;
  /// @brief stream size in bytes
  std::string size;
  /// @brief ???
  int probe_score;

  /**
   * @brief The parsed FFProbe format tags result.
   *
   */
  ProbeResultFormatTags tags;
};

#endif  // PROBE_RESULT_FORMAT
