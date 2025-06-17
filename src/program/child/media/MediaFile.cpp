#include "MediaFile.h"

#include <regex>
#include <stdexcept>
#include <string>
#include <vector>

#include "../../../program/Program.h"
#include "../../../utils/RegexUtils.h"
#include "../../../utils/StringUtils.h"
#include "../../../utils/logging/Logger.h"
#include "../../settings/arguments/video/Quality.h"

MediaFile::MediaFile()
    : conversionName(""),
      conversionNameExt(""),
      conversionFilePath(""),
      conversionFolderPath(""),
      episode(""),
      series(""),
      originalFileNameExt(""),
      originalFullPath(""),
      cwd(""),
      ext(""),
      number(0),
      size(0),
      newSize(0),
      quality(0),
      season(0) {}

MediaFile::MediaFile(uuids::uuid media_id, std::string name, std::string path)
    : media_id(media_id),
      conversionName(""),
      conversionNameExt(""),
      conversionFilePath(""),
      conversionFolderPath(""),
      episode(""),
      series(""),
      originalFileNameExt(name),
      originalFullPath(""),
      cwd(path),
      ext(""),
      number(0),
      size(0),
      newSize(0),
      quality(0),
      season(0) {}

MediaFile::~MediaFile() { LOG_DEBUG("Deconstructing MediaFile"); }

/**
 * Checklist:
 *
 *   Make these all separate private functions
 *
 *   [x] - Original file name
 *   [x] - Original file name path
 *   [x] - Conversion name
 *   [x] - Conversion name ext
 *   [x] - Conversion file path
 *   [x] - Conversion folder path
 *   [x] - Extension
 *   [] - Episode (episodes are matching e00- instead of e00)
 *   [x] - Series
 *   [x] - Quality
 *   [x] - Season
 */

void MediaFile::rename() {
  // pattern for matching media names
  // ex: The.Big.Bang.Theory.S01E01.720p.HDTV.ReEnc-Max.mkv
  // ex: The Big Bang Theory - S01E01 - Pilot.mkv
  // ex: The Big Bang Theory - 1x01 - Pilot.mkv
  std::string mediaPattern =
      R"((.+?)(?:[-\. ]+)(season.?\d{1,}|s\d{1,}).?((?:E|X)[0-9]{2}(?:-(?:E|X)[0-9]{2}|(?:E|X)[0-9]{2})*(?:-(?:E|X)[0-9]{2})?))";

  // if the media name matches the media pattern
  if (RegexUtils::isMatch(this->originalFileNameExt, mediaPattern,
                          std::regex::icase)) {
    LOG_DEBUG("Matched media name: ", this->originalFileNameExt);
    // get all matches of the media name
    std::vector<std::string> media_matches = RegexUtils::getAllMatches(
        this->originalFileNameExt, mediaPattern, std::regex::icase);

    this->resolveExtension(this->originalFileNameExt);

    this->resolveSeries(media_matches[0]);
    this->resolveSeason(media_matches[1]);
    this->resolveEpisode(media_matches[2]);
    this->resolveQuality(this->originalFileNameExt);

    this->resolveOriginalFullPath(this->cwd, this->originalFileNameExt);
    this->resolveConversionPaths(this->series, this->season, this->episode,
                                 this->cwd);

    LOG_DEBUG("Original file:", this->originalFileNameExt);
    LOG_DEBUG("Original path:", this->originalFullPath);
    LOG_DEBUG("Renamed file:", this->conversionName);
    LOG_DEBUG("Renamed file path:", this->conversionFilePath);
    LOG_DEBUG("Renamed folder path:", this->conversionFolderPath);
  } else {
    // TODO: finish
    LOG_DEBUG("Could not match media name: ", this->originalFileNameExt);

    // satisfies the condition of originalFilePath
    this->resolveOriginalFullPath(this->cwd, this->originalFileNameExt);
    // satisfies the condition of ext
    this->resolveExtension(this->originalFileNameExt);
    // satisfies the condition of quality
    this->resolveQuality(this->originalFileNameExt);

    this->conversionName =
        StringUtils::replaceAll(this->originalFileNameExt, this->ext, "");
    this->conversionNameExt = this->originalFileNameExt;
    this->conversionFolderPath = this->cwd + "/converted/";
    this->conversionFilePath =
        this->conversionFolderPath + this->conversionNameExt;
  }
}
// if the pattern doesnt match, the mod file name is the same as the original

void MediaFile::resolveOriginalFullPath(std::string path,
                                        std::string original_filename) {
  if (this->cwd == "\\") {
    this->originalFullPath = "./" + original_filename;
  } else {
    this->originalFullPath = path + "/" + original_filename;
  }
}

void MediaFile::resolveConversionPaths(std::string provided_series,
                                       int provided_season,
                                       std::string provided_episode,
                                       std::string path) {
  ArgumentRegistry& argumentRegistry =
      *Program::settings->childOptionsMap[this->media_id]->argumentRegistry;

  MediaFormat format = argumentRegistry.get_t<Quality>(Command::QUALITY)->get();

  std::string season_number = "";
  std::string program_quality = format.name;
  // TODO: set uuid within media so we can obtain child settings
  // settings->argumentParser->quality.get().name;

  if (provided_season < 10) {
    season_number = "0" + std::to_string(provided_season);
  } else {
    season_number = std::to_string(provided_season);
  }

  std::string compiled_filename = provided_series + " - s" + season_number +
                                  provided_episode + " [" + program_quality +
                                  "]";

  this->conversionName = compiled_filename;
  this->conversionNameExt = compiled_filename + ".mkv";
  this->conversionFolderPath = path + "/" + provided_series + " Season " +
                               std::to_string(provided_season) + "/";
  this->conversionFilePath =
      this->conversionFolderPath + compiled_filename + ".mkv";
}

void MediaFile::resolveExtension(std::string original_filename) {
  std::string found_extension = RegexUtils::getFirstMatch(
      original_filename, R"((\.mkv|\.avi|\.srt|\.idx|\.sub))",
      std::regex::icase);

  if (found_extension != "") {
    this->ext = found_extension;
  } else {
    throw std::runtime_error("Could not find extension for file: " +
                             original_filename);
  }
}

void MediaFile::resolveSeries(std::string series_match) {
  if (series_match == "") {
    throw std::runtime_error("Could not find series name for file: " +
                             this->originalFullPath);
  }

  this->series = StringUtils::replaceAll(series_match, ".", " ");
}

// should be a match of
// season ## || s##
// resolves to an integer

void MediaFile::resolveSeason(std::string season_match) {
  if (season_match == "") {
    throw std::runtime_error("Could not find season number for file: " +
                             this->originalFullPath);
  }

  this->season = std::stoi(StringUtils::replaceAll(
      season_match, std::regex(R"(season|s)", std::regex::icase), ""));
}

// should be a match of
// e## || x##
// e##-e## || x##-x##

void MediaFile::resolveEpisode(std::string episode_match) {
  if (episode_match == "") {
    throw std::runtime_error("Could not find episode number for file: " +
                             this->originalFullPath);
  }

  this->episode =
      StringUtils::replaceAll(episode_match, std::regex(R"([XxE])"), "e");
}

void MediaFile::resolveQuality(std::string original_filename) {
  std::string quality_match = RegexUtils::getFirstMatch(
      original_filename, R"((1080p|720p|480p))", std::regex::icase);

  if (quality_match == "") {
    LOG_DEBUG("Could not find quality for file: ", this->originalFileNameExt);
  } else {
    this->quality = std::stoi(StringUtils::replaceAll(quality_match, "p", ""));
  }
}
