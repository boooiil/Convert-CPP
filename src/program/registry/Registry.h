#ifndef FB8A5CCD_0551_438B_8790_F2C736093027
#define FB8A5CCD_0551_438B_8790_F2C736093027

#include "src/program/generics/JSONSerializable.h"
#include "src/utils/logging/Logger.h"
#include <memory>
#include <nlohmann/json.hpp>
#include <set>
#include <unordered_map>
#include <vector>

template <typename K, typename V> class Registry : public JSONSerializable {
public:
  Registry() = default;
  Registry(Registry &&) = default;
  Registry &operator=(Registry &&) = default;

  virtual ~Registry(void) = default;

  virtual Registry &add(const K &key, std::unique_ptr<V> value) {
    registry[key] = std::move(value);
    return *this;
  }

  virtual Registry &remove(const K &key) {
    registry.erase(key);
    return *this;
  }

  virtual bool has(const K &key) const {
    return registry.find(key) != registry.end();
  }
  virtual bool empty() const { return registry.empty(); }

  virtual V &get(const K &key) {
    auto it = registry.find(key);
    if (it != registry.end())
      return *(it->second);
    LOG_DEBUG("No key of type ", typeid(K).name(), " found in the registry.");
    throw std::runtime_error("Key not found in the registry.");
  }

  virtual const V &get(const K &key) const {
    auto it = registry.find(key);
    if (it != registry.end()) {
      return *(it->second);
    } else {
      LOG_DEBUG("No key of type ", typeid(K).name(), " found in the registry.");
      throw std::runtime_error("Key not found in the registry.");
    }
  }

  virtual std::set<K> keySet() const {
    std::set<K> keys;
    for (const auto &[k, _] : registry)
      keys.insert(k);
    return keys;
  }

  virtual std::vector<std::reference_wrapper<const V>> values() const {
    std::vector<std::reference_wrapper<const V>> vals;
    for (const auto &[_, v] : registry)
      vals.push_back(*v);
    return vals;
  }

  virtual void fromJSON(const nlohmann::json &json) override {};
  virtual nlohmann::json toJSON() override {
    nlohmann::json json;
    // Implementation for serializing to JSON
    return json;
  };

  V &operator[](const K &key) { return get(key); }

  const V &operator[](const K &key) const { return get(key); }

protected:
  std::unordered_map<K, std::unique_ptr<V>> registry;
};

#endif /* FB8A5CCD_0551_438B_8790_F2C736093027 */
