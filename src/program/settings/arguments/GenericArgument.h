
#ifndef GENERIC_ARGUMENT_H
#define GENERIC_ARGUMENT_H
#include <string>

class GenericArgument {
public:
  GenericArgument() : errored(false) {}
  virtual ~GenericArgument() = default;
  virtual void parse(std::string argument) = 0;
  virtual std::string getHelpMessage(void) const = 0;
  virtual std::string getLongFlag(void) const = 0;
  virtual std::string getFlag(void) const = 0;
  virtual std::string getTypeDescriptor(void) const = 0;

  virtual const std::string toString(void) const = 0;
  virtual const bool hasData(void) const = 0;
  virtual const bool isFlag(void) const { return false; }

  void setErrored(bool provided) { this->errored = provided; }
  const bool isErrored(void) { return this->errored; }

private:
  bool errored;
};

#endif // !GENERIC_ARGUMENT_H
