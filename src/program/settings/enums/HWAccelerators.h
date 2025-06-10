/*********************************************************************
 * @file   HWAccelerators.h
 * @brief  Hardware Accelerators enum and map.
 *
 * @author boooiil
 * @date   January 2024
 *********************************************************************/

#if !defined(HWACCELERATORS)
#define HWACCELERATORS

#include <string>
#include <vector>

#include "StringEnumDataHolder.h"

 /**
  * @brief Class for HWAccelerators enum and map.
  *
  */
enum class HWAccelerators {
  INVALID = 0,

  AMD,
  NVIDIA,
  INTEL,
  VULKAN

  /*public:
   static const StringEnumDataHolder<HWAccelerators> NONE;
   static const StringEnumDataHolder<HWAccelerators> AMD;
   static const StringEnumDataHolder<HWAccelerators> NVIDIA;
   static const StringEnumDataHolder<HWAccelerators> INTEL;
   static const StringEnumDataHolder<HWAccelerators> VULKAN;

   static const std::vector<const StringEnumDataHolder<HWAccelerators>*> _all() {
     return {&NONE, &AMD, &NVIDIA, &INTEL, &VULKAN};
   }

   static const StringEnumDataHolder<HWAccelerators> getKey(std::string value) {
     for (auto& item : _all()) {
       if (item->getName() == value) {
         return *item;
       }
     }
     return NONE;
   }*/
};

#endif  // HWACCELERATORS
