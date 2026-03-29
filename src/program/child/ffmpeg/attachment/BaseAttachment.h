#ifndef E22A1483_AF8A_4C83_A7FF_85A71F530D51
#define E22A1483_AF8A_4C83_A7FF_85A71F530D51

#include <set>
#include <string>

class BaseAttachment {
public:
  virtual ~BaseAttachment() = default;
  BaseAttachment() {};

  virtual auto getName() const -> const std::string {
    return "BaseAttachment";
  };
  virtual auto getDisplayName() const -> const std::string {
    return "Base Attachment";
  };
  virtual auto getAliases() const -> const std::set<std::string> { return {}; };

  void setIndex(const int &idx) { this->index = idx; }
  const int getIndex() const { return this->index; }

  void setMapIndex(const int &idx) { this->mapIndex = idx; }
  const int getMapIndex() const { return this->mapIndex; }

private:
  int index;
  int mapIndex;
};

#endif /* E22A1483_AF8A_4C83_A7FF_85A71F530D51 */
