#ifndef BASE_DISPLAY_H
#define BASE_DISPLAY_H

#include "JSONSerializableRunner.h"

class BaseDisplay {
public:
  virtual ~BaseDisplay(void) = default;
  virtual void print(JSONSerializableRunner &ticker) = 0;
  virtual void printDebug(JSONSerializableRunner &ticker) = 0;
  virtual void printInformation(JSONSerializableRunner *ticker,
                                JSONSerializableRunner *runner) = 0;
  virtual void printJSON(void) = 0;
};

#endif // !BASE_DISPLAY_H
