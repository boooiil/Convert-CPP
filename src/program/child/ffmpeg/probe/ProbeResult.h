/*********************************************************************
 * @file   ProbeResult.h
 * @brief  Probe result class header file.
 *
 * @author boooiil
 * @date   January 2024
 *********************************************************************/
#if !defined(PROBE_RESULT)
#define PROBE_RESULT

#include <nlohmann/json_fwd.hpp>
#include <vector>

#include "ProbeResultFormat.h"
#include "ProbeResultStreamAudio.h"
#include "ProbeResultStreamSubtitle.h"
#include "ProbeResultStreamVideo.h"

/**
 * @brief This is the base JSON parse for the ffprobe result.
 *
 */
class ProbeResult {
 public:
  /** This constructor should never be called. */
  ProbeResult();

  /**
   * @brief Construct a new Probe Result object
   *
   * @param[in] JSON - The FFProbe result JSON.
   */
  ProbeResult(const nlohmann::json JSON);
  ~ProbeResult();

  /**
   * @brief The parsed FFProbe format result.
   *
   */
  ProbeResultFormat format;

  /**
   * @brief The parsed FFProbe video streams result.
   *
   */
  std::vector<ProbeResultStreamVideo> videoStreams;

  /**
   * @brief The parsed FFProbe video streams result.
   *
   */
  std::vector<ProbeResultStreamAudio> audioStreams;

  /**
   * @brief The parsed FFProbe subtitle streams result.
   *
   */
  std::vector<ProbeResultStreamSubtitle> subtitleStreams;
};

#endif  // PROBE_RESULT
