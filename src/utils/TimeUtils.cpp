#include "TimeUtils.h"

#include <chrono>
#include <cmath>
#include <ctime>

long TimeUtils::getTime() { return 0; };

long TimeUtils::getEpoch() {
  auto currentTime = std::chrono::system_clock::now();

  // Convert the time point to time_t (seconds since epoch)
  std::time_t epochTime = std::chrono::system_clock::to_time_t(currentTime);

  // Using long to store epoch time
  long epochTimeLong = static_cast<long>(epochTime);

  return epochTimeLong;
};

std::string TimeUtils::durationFormat(long ms) {
  std::string duration("");

  long s = static_cast<long>(floor(ms / 1000.0));
  long m = 0;
  long h = 0;

  // get minutes and subtract from seconds
  m = static_cast<long>(floor(s / 60.0));
  s -= m * 60;

  // get hours and subtract from minutes
  h = static_cast<long>(floor(m / 60.0));
  m -= h * 60;

  if (h > 0) {
    if (h < 10) {
      duration += "0" + std::to_string(h) + ":";
    } else if (h > 0) {
      duration += std::to_string(h) + ":";
    } else {
      duration += "00:";
    }
  }
  if (m > 0 || h) {
    if (m < 10) {
      duration += "0" + std::to_string(m) + ":";
    } else if (m > 0) {
      duration += std::to_string(m) + ":";
    } else {
      duration += "00:";
    }
  }
  if (s > 0 || h || m) {
    if (s < 10) {
      duration += "0" + std::to_string(s);
    } else {
      duration += std::to_string(s);
    }
  }

  return duration;
};

std::string TimeUtils::dateFormat(long epoch) {
  // Convert the time point to time_t (seconds since epoch)
  std::time_t epochTime = epoch;

  // Convert to local time
  std::tm localTime;

#ifdef _WIN32
  localtime_s(&localTime, &epochTime);
#else
  localtime_r(&epochTime, &localTime);
#endif

  // Format the time as a string
  char timeString[80];
  std::strftime(timeString, 80, "%H:%M:%S %Y-%m-%d", &localTime);

  return std::string(timeString);
};

std::string TimeUtils::timeFormat(long epoch) {
  // Convert the time point to time_t (seconds since epoch)
  std::time_t epochTime = epoch;

  // Convert to local time
  std::tm localTime;

#ifdef _WIN32
  localtime_s(&localTime, &epochTime);
#else
  localtime_r(&epochTime, &localTime);
#endif

  // Format the time as a string
  char timeString[80];
  std::strftime(timeString, 80, "%H:%M:%S-%p", &localTime);

  return std::string(timeString);
};