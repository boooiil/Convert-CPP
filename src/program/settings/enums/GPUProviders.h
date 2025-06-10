#ifndef GPU_PROVIDERS_H
#define GPU_PROVIDERS_H

#include <string>
#include <vector>

#include "StringEnumDataHolder.h"

enum class GPUProviders {
  INVALID = 0,

  NVIDIA,
  AMD,
  INTEL
  //public:
  //  static const StringEnumDataHolder<GPUProviders> NVIDIA;
  //  static const StringEnumDataHolder<GPUProviders> AMD;
  //  static const StringEnumDataHolder<GPUProviders> INTEL;
  //  static const StringEnumDataHolder<GPUProviders> UNKNOWN;
  //
  //  static const StringEnumDataHolder<GPUProviders> getPreferred(
  //      std::vector<StringEnumDataHolder<GPUProviders>>);
  //
  //  static const std::vector<const StringEnumDataHolder<GPUProviders>*> _all() {
  //    return {&NVIDIA, &AMD, &INTEL, &UNKNOWN};
  //  };
  //
  //  static const StringEnumDataHolder<GPUProviders> getKey(std::string value) {
  //    for (auto& item : _all()) {
  //      if (item->getName() == value) {
  //        return *item;
  //      }
  //    }
  //    return UNKNOWN;
  //  }
};

#endif  // GPU_PROVIDERS_H