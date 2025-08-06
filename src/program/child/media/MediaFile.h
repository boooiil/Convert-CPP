/*********************************************************************
 * @file   MediaFile.h
 * @brief  MediaFile class header.
 *
 * @author boooiil
 * @date   January 2024
 *********************************************************************/

#ifndef MEDIA_FILE
#define MEDIA_FILE

#include <uuid.h>

#include <string>

/**
 * @brief Hold information about a media file.
 */
class MediaFile {
public:
  /**
   * @brief Construct a new Media File object.
   *
   */
  MediaFile(void);

  /**
   * @brief Construct a new Media File object.
   *
   * @param[in] id - Unique identifier for the media file.
   * @param[in] name - Name of the file.
   * @param[in] path - Path to the file (generally CWD).
   */
  MediaFile(uuids::uuid media_id, std::string name, std::string path);

  ~MediaFile(void);

  /// @brief Unique identifier for the media file.
  uuids::uuid media_id;
  /// @brief CWD
  std::string cwd;
  /// @brief Original filename with extension
  std::string originalFileNameExt;
  /// @brief Original file path with filename and extension
  std::string originalFullPath;
  /// @brief Result of modifiedFileName
  std::string conversionName;
  /// @brief Result of modifiedFileNameExt
  std::string conversionNameExt;
  /// @brief  Path resolving to cwd/{series} Season {##}/{conversionNameExt}
  std::string conversionFilePath;
  /// @brief Path resolving to cwd/{series} Season {##}/
  std::string conversionFolderPath;
  /// @brief Episode number in the series
  std::string episode;
  /// @brief Series name
  std::string series;
  /// @brief File extension
  std::string ext;

  int number;          /// @brief Unsure?
  std::size_t size;    /// @brief File size in bytes
  std::size_t newSize; /// @brief File size in bytes after conversion
  int quality;         /// @brief Quality of the file
  int season;          /// @brief Season number

  /**
   * @brief Rename the current media name to a standard format.
   *
   * @param[out] container - Ptr to the Container object.
   */
  void rename();

private:
  /**
   * @brief Compile the full path of the original file.
   *
   */
  void resolveOriginalFullPath(std::string path, std::string original_filename);
  /**
   * @brief Compile the conversion paths.
   *
   * @param[in] series      - Series name.
   * @param[in] season      - Season number.
   * @param[in] episode     - Episode number.
   * @param[in] quality     - Chosen quality.
   */
  void resolveConversionPaths(std::string provided_series, int provided_season,
                              std::string provided_episode, std::string path);

  /**
   * @brief Compile the extension.
   *
   * @param[in] original_filename - The original file name.
   */
  void resolveExtension(std::string original_filename);

  /**
   * @brief Compile the series.
   *
   * @param[in] series_match - Provided match from the regex.
   */
  void resolveSeries(std::string series_match);

  /**
   * @brief Compile the season.
   *
   * @param[in] season_match - Provided match from the regex.
   */
  void resolveSeason(std::string season_match);

  /**
   * @brief Compile the episode.
   *
   * @param[in] episode_match - Provided match from the regex.
   */
  void resolveEpisode(std::string episode_match);

  /**
   * @brief Compile the quality (this is unused).
   *
   * @param[in] original_filename - The original file name.
   */
  void resolveQuality(std::string original_filename);
};

#endif // !MEDIA_FILE
