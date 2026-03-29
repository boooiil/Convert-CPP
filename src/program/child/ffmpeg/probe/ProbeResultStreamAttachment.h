#ifndef A69889F5_FD71_4C04_9FAB_CF2CF3DA2370
#define A69889F5_FD71_4C04_9FAB_CF2CF3DA2370

#include "ProbeResultStream.h"

class ProbeResultStreamAttachment : public ProbeResultStream {
public:
  ProbeResultStreamAttachment(nlohmann::json JSON);
  ~ProbeResultStreamAttachment() = default;

  int duration_ts;
  std::string duration;
};

#endif /* A69889F5_FD71_4C04_9FAB_CF2CF3DA2370 */
