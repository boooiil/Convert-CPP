#ifndef D4034739_C790_49D1_921C_5C3445138713
#define D4034739_C790_49D1_921C_5C3445138713

#include "src/program/settings/enums/Encoders_N.h"
#include "src/program/settings/enums/GPUProviders_N.h"
#include "src/program/settings/enums/HWAccelerators_N.h"

struct HardwareCapabilities {
  /// @brief List of supported encoders by the program.
  std::vector<Encoders_N::Encoders> supportedEncoders;
  /// @brief List of supported hardware accelerators by the program.
  std::vector<HWAccelerators_N::HWAccelerators> supportedHWAccel;
  // TODO: change this to a map vvvv
  /// @brief The current GPU of the user's machine
  std::vector<GPUProviders_N::GPUProviders> GPU_Providers;
  /// @brief The preferred GPU provider for the user.
  GPUProviders_N::GPUProviders preferredGPUProvider;

  const nlohmann::json toJSON() const;
  static const HardwareCapabilities detect(void);
};

#endif /* D4034739_C790_49D1_921C_5C3445138713 */
