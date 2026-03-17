#include "ParentOptions.h"

#include "../../../utils/logging/Logger.h"
#include "../arguments/IntegerArgument.h"
#include <nlohmann/json.hpp>

ParentOptions::ParentOptions(void) : argumentRegistry(new ArgumentRegistry()) {}

ParentOptions::~ParentOptions(void) {
  LOG_DEBUG("Destroying ParentOptions...");
  LOG_DEBUG("Expecting to destroy { ArgumentRegistry }");
  if (this->argumentRegistry != nullptr) {
    delete this->argumentRegistry;
  }
  // Destructor
}

void ParentOptions::prepare(void) {
  // Prepare the options
  // amount

  argumentRegistry->add(Command_N::AMOUNT,
                        std::make_unique<IntegerArgument>(
                            "Amount of media to process", "-a", "--amount", 1));
}

void ParentOptions::parse(std::vector<std::string> &args) {
  this->i_args = args;
  this->argumentRegistry->parse(args);
}

void ParentOptions::validate(void) {
  // Validate the options
}

void ParentOptions::fromJSON(const nlohmann::json json) { (void)json; }

nlohmann::json ParentOptions::toJSON(void) {
  nlohmann::json parentOptions;

  parentOptions["arguments"] = this->argumentRegistry->toJSON();
  parentOptions["i_args"] = this->i_args;

  return parentOptions;
}
