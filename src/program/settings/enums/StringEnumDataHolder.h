#ifndef STRING_ENUM_DATA_HOLDER_H
#define STRING_ENUM_DATA_HOLDER_H

#include <string>

class StringEnumDataHolder {
public:
  StringEnumDataHolder() = default;

  StringEnumDataHolder(std::string _name, std::string _long_name,
                       std::string _description)
      : name(_name), description(_description), long_name(_long_name) {}

  std::string getName() const { return name; }
  std::string getDescription() const { return description; }
  std::string getLongName() const { return long_name; }

  // Overload the equality operator for comparison
  bool operator==(const StringEnumDataHolder &other) const {
    return name == other.name;
  }

  bool operator!=(const StringEnumDataHolder &other) const {
    return name != other.name;
  }

  std::string operator+(const std::string &other) const { return name + other; }

  operator std::string() const { return name; }

private:
  std::string name;
  std::string description;
  std::string long_name;
};

#endif // STRING_ENUM_DATA_HOLDER_H