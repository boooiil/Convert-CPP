/*********************************************************************
 * @file   ProbeResultFormatTags.h
 * @brief  Probe Result Format Tags header.
 *
 * @author boooiil
 * @date   January 2024
 *********************************************************************/

#if !defined(PROBE_RESULT_FORMAT_TAGS)
#define PROBE_RESULT_FORMAT_TAGS

#include <nlohmann/json_fwd.hpp>
#include <string>

/**
 * @brief This class represents the FFProbe format tags.
 */
class ProbeResultFormatTags {
 public:
  /** This constructor should never be called. */
  ProbeResultFormatTags();

  /**
   * @brief Construct a new Probe Result Format Tags object
   *
   * @param[in] JSON - The FFProbe tag JSON.
   */
  ProbeResultFormatTags(const nlohmann::json JSON);
  ~ProbeResultFormatTags();

  /// @brief full encoder name
  std::string ENCODER;
};

#endif  // PROBE_RESULT_FORMAT_TAGS
