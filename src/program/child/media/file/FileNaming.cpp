#include "FileNaming.h"
#include "nlohmann/json.hpp"
#include "src/utils/RegexUtils.h"
#include "src/utils/StringUtils.h"
#include "src/utils/logging/Logger.h"
#include <string>

FileNaming::FileNaming(const std::string &file_name,
                       const std::filesystem::path &folder_path,
                       const std::string &wanted_quality,
                       const std::string &wanted_extension)
    : folder_path(folder_path), original_name_ext(file_name),
      original_full_path(folder_path / file_name),
      extension(original_full_path.extension().string()),
      wanted_quality(wanted_quality), wanted_extension(wanted_extension) {

      };

// TODO: do
void FileNaming::rename() {

  std::string mediaPattern =
      R"((.+?)(?:[-\. ]+)(season.?\d{1,}|s\d{1,}).?((?:E|X)[0-9]{2}(?:-(?:E|X)[0-9]{2}|(?:E|X)[0-9]{2})*(?:-(?:E|X)[0-9]{2})?))";

  if (RegexUtils::isMatch(this->original_name_ext, mediaPattern,
                          std::regex::icase)) {
    LOG_DEBUG("Matched media name: ", this->original_name_ext);
    std::vector<std::string> media_matches = RegexUtils::getAllMatches(
        this->original_name_ext, mediaPattern, std::regex::icase);
    std::string quality_match = RegexUtils::getFirstMatch(
        this->original_name_ext, R"((2160p|1080p|720p|480p))",
        std::regex::icase);

    if (media_matches[0] == "" || media_matches[1] == "" ||
        media_matches[2] == "") {
      throw std::runtime_error("Matcher failed for :" +
                               this->original_full_path.string());
    }

    if (quality_match == "") {
      LOG_DEBUG("Could not find quality for file: ", this->original_name_ext);
    } else {
      this->resolution_height =
          std::stoi(StringUtils::replaceAll(quality_match, "p", ""));
    }

    this->series = StringUtils::replaceAll(media_matches[0], ".", " ");
    this->season_no = std::stoi(StringUtils::replaceAll(
        media_matches[1], std::regex(R"(season|s)", std::regex::icase), ""));
    this->episode =
        StringUtils::replaceAll(media_matches[2], std::regex(R"([XxE])"), "e");

    if (season_no > 10) {
      this->conversion_name = series + " - s" + std::to_string(season_no) +
                              episode + " [" + wanted_quality + "]";
    } else {
      this->conversion_name = series + " - s0" + std::to_string(season_no) +
                              episode + " [" + wanted_quality + "]";
    }

    this->conversion_name_ext = this->conversion_name + "." + wanted_extension;
    this->conversion_folder_path =
        this->folder_path / (series + " Season " + std::to_string(season_no));
    this->conversion_full_path =
        this->conversion_folder_path / this->conversion_name_ext;
  } else {
    LOG_DEBUG("Could not match media name: ", this->original_full_path);
    this->conversion_name = this->original_name;
    this->conversion_name_ext = this->original_name + wanted_extension;
    this->conversion_folder_path = this->folder_path / "converted";
    this->conversion_full_path =
        this->conversion_folder_path / this->conversion_name_ext;
  }

  LOG_VERBOSE("Original file:", this->original_name);
  LOG_DEBUG("Original path:", this->original_full_path);
  LOG_VERBOSE("Renamed file:", this->conversion_name);
  LOG_DEBUG("Renamed file path:", this->conversion_full_path);
  LOG_VERBOSE("Renamed folder path:", this->conversion_folder_path);
}

void FileNaming::fromJSON(const nlohmann::json &json) {
  this->original_name_ext = json["original_name_ext"];
  this->original_full_path =
      std::filesystem::path(std::string(json["original_full_path"]));
  this->conversion_name = json["conversion_name"];
  this->conversion_name_ext = json["conversion_name_ext"];
  this->conversion_full_path =
      std::filesystem::path(std::string(json["conversion_full_path"]));
  this->conversion_folder_path =
      std::filesystem::path(std::string(json["conversion_folder_path"]));
  this->episode = json["episode"];
  this->extension = json["extension"];
  this->resolution_height = json["resolution_height"];
  this->season_no = json["season_no"];
}

nlohmann::json FileNaming::toJSON() {
  nlohmann::json json;
  json["original_name_ext"] = original_name_ext;
  json["original_full_path"] = original_full_path;
  json["conversion_name"] = conversion_name;
  json["conversion_name_ext"] = conversion_name_ext;
  json["conversion_full_path"] = conversion_full_path;
  json["conversion_folder_path"] = conversion_folder_path;
  json["episode"] = episode;
  json["series"] = series;
  json["resolution_height"] = resolution_height;
  json["season_no"] = season_no;
  return json;
}