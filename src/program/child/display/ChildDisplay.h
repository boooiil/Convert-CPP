#ifndef CHILD_DISPLAY_H
#define CHILD_DISPLAY_H

#include "../../generics/GenericDisplay.h"
#include "../../ticker/NTicker.h"
#include "../Child.h"

class ChildDisplay : public GenericDisplay<NTicker, Child> {
 public:
  void print(void);
  void printDebug(void);
  void printInformationTyped(NTicker* ticker, Child* child);
  void printJSON(void);
};

#endif  // !CHILD_DISPLAY_H
