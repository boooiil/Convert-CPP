#include "ParentDisplay.h"

#include "../../../utils/logging/LogColor.h"
#include "../../../utils/logging/Logger.h"
#include "../../child/display/ChildDisplay.h"
#include "../../ticker/NTicker.h"
#include "../Parent.h"
#include "src/program/generics/JSONSerializableRunner.h"

// NOLINTBEGIN(modernize-return-braced-init-list)
inline auto tab(int spaces) -> std::string {
  return std::string(spaces * 2, ' ');
}
// NOLINTEND(modernize-return-braced-init-list)
void ParentDisplay::print(JSONSerializableRunner &ticker) {
  // TODO: finish
}

void ParentDisplay::printDebug(JSONSerializableRunner &ticker) {
  // TODO: finish
}

void ParentDisplay::printInformationTyped(NTicker *ticker, Parent *parent) {
  assert(ticker != nullptr);
  assert(parent != nullptr);

  LOG_DEBUG(Logger::Priority::INFO, "Printing Information");

  LOG(Logger::Priority::INFO, "Parent size:", parent->pending.size());

  LOG(Logger::Priority::INFO, LogColor::fgBlack("Black"),
      LogColor::fgRed("Red"), LogColor::fgGreen("Green"),
      LogColor::fgGray("Gray"), LogColor::fgYellow("Yellow"),
      LogColor::fgBlue("Blue"), LogColor::fgOrange("Orange"),
      LogColor::fgMagenta("Magenta"), LogColor::fgCyan("Cyan"),
      LogColor::fgWhite("White"));

  if (parent->pending.empty()) {
    LOG(Logger::Priority::WARNING, LogColor::fgRed("No media files found."));
    return;
  }

  /********************************************
   *  ITERATE TRHOUGH PARENT PENDING CHILDREN  *
   *********************************************/
  std::queue<Child *> child_t_queue;
  while (!parent->pending.empty()) {
    Child *child = parent->pending.front();
    parent->pending.pop();

    if (child->pending.empty()) {
      LOG_DEBUG(Logger::Priority::INFO, "Child has no pending media files.");
      continue;
    }

    ChildDisplay childDisplay;
    childDisplay.printInformation(ticker, child);

    // push the child back into the parent's pending queue
    child_t_queue.push(child);
  }

  // assign the temp child queue to the parent's pending queue
  parent->pending = child_t_queue;
}

void ParentDisplay::printJSON() {
  // TODO: finish
}
