/*********************************************************************
 * @file   ProbeResultStreamAudio.h
 * @brief  Probe result stream audio class header.
 *
 * @author boooiil
 * @date   January 2024
 *********************************************************************/

#if !defined(PROBE_RESULT_STREAM_AUDIO)
#define PROBE_RESULT_STREAM_AUDIO

#include <nlohmann/json_fwd.hpp>
#include <string>

#include "ProbeResultStream.h"

/**
 * @brief Probe result stream audio class.
 */
class ProbeResultStreamAudio : public ProbeResultStream {
public:
  /** This constructor should never be called. */
  ProbeResultStreamAudio();

  /**
   * @brief Construct a new Probe Result Stream Audio object
   *
   * @param[in] JSON - The FFProbe stream JSON.
   */
  ProbeResultStreamAudio(const nlohmann::json JSON);
  ~ProbeResultStreamAudio();

  /// @brief sample format
  std::string sample_fmt;
  /// @brief sample rate
  int sample_rate;
  /// @brief audio channels
  int channels;
  /// @brief channel layout
  std::string channel_layout;
  /// @brief bits per sample
  int bits_per_sample;
  /// @brief audio padding
  int initial_padding;
};

#endif // PROBE_RESULT_STREAM_AUDIO
