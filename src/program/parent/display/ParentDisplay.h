#ifndef PARENT_DISPLAY_H
#define PARENT_DISPLAY_H

#include "../../generics/GenericDisplay.h"
#include "../../ticker/NTicker.h"
#include "../Parent.h"

class ParentDisplay : public GenericDisplay<NTicker, Parent> {
 public:
  void print(void);
  void printDebug(void);
  void printInformationTyped(NTicker* ticker, Parent* parent);
  void printJSON(void);
};

#endif  // !PARENT_DISPLAY_H
