#ifndef CE9484DC_DC5C_41CD_8A7B_39C86725CF0C
#define CE9484DC_DC5C_41CD_8A7B_39C86725CF0C

#include <string>
#include <typeinfo>
#include <unordered_set>

class Definition {
public:
  Definition(const std::string &name,
             const std::unordered_set<std::string> &aliases,
             const std::string &short_description,
             const std::string &long_description)
      : name(name), aliases(aliases), short_description(short_description),
        long_description(long_description) {}

  Definition(const std::string &name,
             const std::unordered_set<std::string> &aliases,
             const std::string &short_description,
             const std::string &long_description, int enum_assoc)
      : name(name), aliases(aliases), short_description(short_description),
        long_description(long_description), enum_assoc(enum_assoc) {}

  Definition(const std::string &name,
             const std::unordered_set<std::string> &aliases,
             const std::string &short_description,
             const std::string &long_description, int enum_assoc,
             const std::type_info &enum_type)
      : name(name), aliases(aliases), short_description(short_description),
        long_description(long_description), enum_assoc(enum_assoc),
        enum_type(enum_type) {}

  virtual ~Definition() = default;

  const std::string &getName() const { return name; }
  const std::unordered_set<std::string> &getAliases() const { return aliases; }
  const std::string &getShortDescription() const { return short_description; }
  const std::string &getLongDescription() const { return long_description; }
  const int getEnumAssoc() const { return enum_assoc; }
  const std::type_info &getEnumType() const { return enum_type; }

  operator std::string() const { return name; }
  operator const char *() const { return name.c_str(); }

  template <typename T>
    requires std::is_enum_v<T>
  const T getEnumAssoc() const {
    return static_cast<T>(enum_assoc);
  }

private:
  const std::string name;
  const std::unordered_set<std::string> aliases;
  const std::string short_description;
  const std::string long_description;
  const int enum_assoc = -1;
  const std::type_info &enum_type = typeid(void);
};

inline std::string operator+(const char *lhs, const Definition &rhs) {
  return std::string(lhs) + rhs.getName();
}

#endif /* CE9484DC_DC5C_41CD_8A7B_39C86725CF0C */
