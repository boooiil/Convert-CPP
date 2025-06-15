#ifndef VECTOR_ARGUMENT_H
#define VECTOR_ARGUMENT_H

#include <exception>
#include <string>
#include <type_traits>
#include <typeinfo>
#include <vector>

#include "../../../utils/ListUtils.h"
#include "../../../utils/logging/Logger.h"
#include "BaseArgument.h"

template <typename T>
class VectorArgument : public BaseArgument<std::vector<T>> {
 public:
  VectorArgument(std::string _helpMessage, std::string _flag,
                 std::string _longFlag, std::vector<T> data)
      : BaseArgument<std::vector<T>>(_helpMessage, _flag, _longFlag, data) {};

  ~VectorArgument(void) {};

  void parse(std::string argument) override {
    if (argument.empty()) {
      LOG_DEBUG("argument was empty");
      this->setErrored(true);
      return;
    }

    try {
      for (std::string index : ListUtils::splitv(argument, ",")) {
        if constexpr (std::is_same<T, int>::value) {
          this->value.push_back(std::stoi(index));

        } else if constexpr (std::is_same<T, std::string>::value) {
          this->value.push_back(index);
        }
      }
    } catch (const std::exception& e) {
      LOG_DEBUG("Failed to parse vector argument: ", argument, "\n", "of type",
                typeid(T).name(), e.what());
      this->setErrored(true);
      return;
    }
  };

  // Convert the vector to a string representation
  const std::string toString(void) const override {
    std::string result = "[";
    for (const auto& elem : this->value) {
      if constexpr (std::is_same<T, int>::value) {
        result += std::to_string(elem);
      } else if constexpr (std::is_same<T, std::string>::value) {
        result += elem;
      }
      result += ", ";
    }
    if (!this->value.empty()) {
      result.pop_back();  // Remove last space
      result.pop_back();  // Remove last comma
    }
    result += "]";
    return result;
  }

  const bool hasData(void) const override { return !this->value.empty(); }
};
#endif  // VECTOR_ARGUMENT_H