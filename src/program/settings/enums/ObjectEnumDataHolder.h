#ifndef OBJECT_ENUM_DATA_HOLDER_H
#define OBJECT_ENUM_DATA_HOLDER_H

#include <string>

template <typename T>
class ObjectEnumDataHolder {
public:
  ObjectEnumDataHolder(std::string _name, T _value)
    : name(_name), value(_value) {}

  std::string getName() const { return name; }

  // Overload the equality operator for comparison
  bool operator==(const ObjectEnumDataHolder& other) const {
    return name == other.name;
  }

  bool operator!=(const ObjectEnumDataHolder& other) const {
    return name != other.name;
  }

  std::string operator+(const std::string& other) const { return name + other; }

  operator std::string const(void) { return name; }

private:
  std::string name;
  T value;
};

#endif  // OBJECT_ENUM_DATA_HOLDER_H