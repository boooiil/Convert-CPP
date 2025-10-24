#ifndef FB8A5CCD_0551_438B_8790_F2C736093027
#define FB8A5CCD_0551_438B_8790_F2C736093027

#include "src/program/generics/JSONSerializable.h"
#include "src/utils/logging/Logger.h"
#include <memory>
#include <nlohmann/json.hpp>
#include <set>
#include <unordered_map>

template <typename K, typename V> class Registry : public JSONSerializable {
public:
  Registry() = default;
  Registry(Registry &&) = default;
  Registry &operator=(Registry &&) = default;

  virtual ~Registry(void) = default;

  Registry &add(const K &key, std::unique_ptr<V> value) {
    registry[key] = std::move(value);
    return *this;
  }

  Registry &remove(const K &key) {
    registry.erase(key);
    return *this;
  }

  bool has(const K &key) const { return registry.find(key) != registry.end(); }

  V *get(const K &key) {
    auto it = registry.find(key);

    if (it != registry.end()) {
      return it->second.get();
    } else {
      LOG_DEBUG("There was no key ", std::string(typeid(key).name()),
                "in the registry.");
      return nullptr;
    }
  }

  const V *get(const K &key) const {
    auto it = registry.find(key);

    if (it != registry.end()) {
      return it->second.get();
    } else {
      LOG_DEBUG("There was no key ", std::string(typeid(key).name()),
                "in the registry.");
      return nullptr;
    }
  }

  std::set<K> keySet() const {
    std::set<K> keys;
    for (const auto &[k, _] : registry)
      keys.insert(k);
    return keys;
  }

  std::set<V *> values() const {
    std::set<V *> vals;
    for (const auto &[_, v] : registry)
      vals.insert(v.get());
    return vals;
  }

  virtual void fromJSON(const nlohmann::json) override {};
  virtual nlohmann::json toJSON() override {
    nlohmann::json json;
    // Implementation for serializing to JSON
    return json;
  };

private:
  std::unordered_map<K, std::unique_ptr<V>> registry;
};

#endif /* FB8A5CCD_0551_438B_8790_F2C736093027 */
