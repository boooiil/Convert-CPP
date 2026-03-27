#ifndef B111914B_1703_4DE5_AB12_E48D1B4E30E2
#define B111914B_1703_4DE5_AB12_E48D1B4E30E2

#include "HardwareCapabilities.h"
#include "PlatformInfo.h"

struct RuntimeEnvironment {
  const HardwareCapabilities hw_cap;
  const PlatformInfo platform_info;

  nlohmann::json toJSON() const;
  static RuntimeEnvironment *detect(void);
};
#endif /* B111914B_1703_4DE5_AB12_E48D1B4E30E2 */
