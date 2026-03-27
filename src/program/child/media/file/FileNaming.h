#ifndef AD1CC83C_16AD_4752_A9EA_84EF154F6B89
#define AD1CC83C_16AD_4752_A9EA_84EF154F6B89

#include "src/program/generics/JSONSerializable.h"
#include <filesystem>
#include <string>

class Media;

class FileNaming : JSONSerializable {

public:
  FileNaming(const std::string &file_name,
             const std::filesystem::path &folder_path,
             const std::string &wanted_quality,
             const std::string &wanted_extension);

  std::filesystem::path folder_path;
  std::string original_name;
  std::string original_name_ext;
  std::filesystem::path original_full_path;
  std::string conversion_name;
  std::string conversion_name_ext;
  std::filesystem::path conversion_full_path;
  std::filesystem::path conversion_folder_path;
  std::string episode;
  std::string series;
  std::string extension;

  // formerly quality
  int resolution_height;
  int season_no;

  void rename();

  void fromJSON(const nlohmann::json &json) override;
  nlohmann::json toJSON() override;

private:
  std::string wanted_quality;
  std::string wanted_extension;
};

#endif /* AD1CC83C_16AD_4752_A9EA_84EF154F6B89 */
