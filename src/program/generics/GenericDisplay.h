#ifndef GENERIC_DISPLAY_H
#define GENERIC_DISPLAY_H

#include <cassert>
#include "BaseDisplay.h"

template<typename Ticker, typename Runner>
class GenericDisplay : public BaseDisplay {
 public:
  virtual ~GenericDisplay(void) = default;
  virtual void print(void) = 0;
  virtual void printDebug(void) = 0;
  void printInformation(JSONSerializableRunner* ticker, JSONSerializableRunner* runner) {
	auto* typedTicker = dynamic_cast<Ticker*>(ticker);
  auto* typedRunner = dynamic_cast<Runner*>(runner);

  assert(typedTicker != nullptr && "Ticker is not of the correct type");
  assert(typedRunner != nullptr && "Runner is not of the correct type");

  printInformationTyped(typedTicker, typedRunner);
  };
  virtual void printInformationTyped(Ticker* ticker, Runner* runner) = 0;
  virtual void printJSON(void) = 0;
};
#endif  // !GENERIC_DISPLAY_H
