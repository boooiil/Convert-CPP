#ifndef F9B0DDC7_4CF4_482D_A58F_F1E4D6E16889
#define F9B0DDC7_4CF4_482D_A58F_F1E4D6E16889

#include "src/program/settings/enums/Platform_N.h"

struct PlatformInfo {
  /// @brief The current operating system of the user's machine
  const Platform_N::Platform platform;
  const bool x64_supported;

  const nlohmann::json toJSON() const;
  static const PlatformInfo detect(void);
};

#endif /* F9B0DDC7_4CF4_482D_A58F_F1E4D6E16889 */
