#ifndef E3437987_8017_4B08_A18C_70280F873AD1
#define E3437987_8017_4B08_A18C_70280F873AD1

#include "FileNaming.h"
#include "FileProcessingInfo.h"
#include "FileVideoInfo.h"
#include "nlohmann/json.hpp"
#include "src/program/generics/JSONSerializable.h"

struct FileContainer : JSONSerializable {
  FileContainer(const std::string &file_name,
                const std::filesystem::path &file_path,
                const std::string &wanted_quality,
                const std::string &wanted_extension)
      : naming(file_name, file_path, wanted_quality, wanted_extension) {};
  FileNaming naming;
  FileProcessingInfo processing_info;
  FileVideoInfo video_info;

  void fromJSON(const nlohmann::json &json) override {
    naming.fromJSON(json["naming"]);
    processing_info.fromJSON(json["processing_info"]);
    video_info.fromJSON(json["video_info"]);
  }

  nlohmann::json toJSON() override {
    nlohmann::json json;
    json["naming"] = naming.toJSON();
    json["processing_info"] = processing_info.toJSON();
    json["video_info"] = video_info.toJSON();
    return json;
  }
};

#endif /* E3437987_8017_4B08_A18C_70280F873AD1 */
