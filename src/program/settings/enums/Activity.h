/*********************************************************************
 * @file   Activity.h
 * @brief  Activity class header file.
 *
 * @author boooiil
 * @date   January 2024
 *********************************************************************/

#ifndef ACTIVITY_H
#define ACTIVITY_H

#include <string>
#include <vector>

#include "StringEnumDataHolder.h"

/**
 * @brief Enum for activity types and utility functions.
 */
enum class Activity {
  INVALID = 0,

  CONVERT,
  FAILED,
  FAILED_CODEC,
  FAILED_CONTAINER,
  FAILED_CORRUPT,
  FAILED_FILE,
  FAILED_FILE_MISSING,
  FAILED_FILE_PERMISSIONS,
  FAILED_FILE_NOT_RECOGNIZED,
  FAILED_HARDWARE,
  FAILED_INVALID_AUDIO_CHANNELS,
  FAILED_INVALID_AUDIO_STREAMS,
  FAILED_INVALID_DURATION_SS,
  FAILED_INVALID_DURATION_TO,
  FAILED_INVALID_ENCODER,
  FAILED_JSON_PARSE,
  FAILED_SYSTEM,
  FINISHED,
  STATISTICS,
  WAITING,
  WAITING_CONVERT,
  WAITING_STATISTICS,
  WAITING_VALIDATE,
  VALIDATE
  /*public:
   static const Activity CONVERT;
   static const Activity FAILED;
   static const Activity FAILED_CODEC;
   static const Activity FAILED_CONTAINER;
   static const Activity FAILED_CORRUPT;
   static const Activity FAILED_FILE;
   static const Activity FAILED_FILE_MISSING;
   static const Activity FAILED_FILE_PERMISSIONS;
   static const Activity FAILED_FILE_NOT_RECOGNIZED;
   static const Activity FAILED_HARDWARE;
   static const Activity FAILED_INVALID_AUDIO_CHANNELS;
   static const Activity FAILED_INVALID_AUDIO_STREAMS;
   static const Activity FAILED_INVALID_DURATION_SS;
   static const Activity FAILED_INVALID_DURATION_TO;
   static const Activity FAILED_INVALID_ENCODER;
   static const Activity FAILED_JSON_PARSE;
   static const Activity FAILED_SYSTEM;
   static const Activity FINISHED;
   static const Activity STATISTICS;
   static const Activity WAITING;
   static const Activity WAITING_CONVERT;
   static const Activity WAITING_STATISTICS;
   static const Activity WAITING_VALIDATE;
   static const Activity VALIDATE;

   static const std::vector<const Activity*> _all() {
     return {&CONVERT,
             &FAILED,
             &FAILED_CODEC,
             &FAILED_CONTAINER,
             &FAILED_CORRUPT,
             &FAILED_FILE,
             &FAILED_FILE_MISSING,
             &FAILED_FILE_PERMISSIONS,
             &FAILED_FILE_NOT_RECOGNIZED,
             &FAILED_HARDWARE,
             &FAILED_INVALID_AUDIO_CHANNELS,
             &FAILED_INVALID_AUDIO_STREAMS,
             &FAILED_INVALID_DURATION_SS,
             &FAILED_INVALID_DURATION_TO,
             &FAILED_INVALID_ENCODER,
             &FAILED_JSON_PARSE,
             &FAILED_SYSTEM,
             &FINISHED,
             &STATISTICS,
             &WAITING,
             &WAITING_CONVERT,
             &WAITING_STATISTICS,
             &WAITING_VALIDATE,
             &VALIDATE};
   };

   static const Activity getKey(std::string value) {
     for (auto& item : _all()) {
       if (item->getName() == value) {
         return *item;
       }
     }
     return FAILED_SYSTEM;
   }*/
};

#endif  // !ACTIVITY_H
