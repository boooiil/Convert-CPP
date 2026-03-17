/*********************************************************************
 * @file   MediaFormat.h
 * @brief  Media format class header file.
 *
 * @author boooiil
 * @date   January 2024
 *********************************************************************/

#ifndef MEDIA_FORMAT
#define MEDIA_FORMAT

#include <string>

/**
 * @brief Hold information about a media format.
 */
class MediaFormat {
 public:
  MediaFormat();
  /**
   * @brief Construct a new Media Format object.
   *
   * @param[in] name    - Name of the format.
   * @param[in] crf     - Constant rate factor.
   * @param[in] bitrate - Bitrate of the format.
   * @param[in] min     - Minimum bitrate of the format.
   * @param[in] max     - Maximum bitrate of the format.
   * @param[in] width   - Width of the format.
   * @param[in] height  - Height of the format.
   * @param[in] crop    - Crop of the format.
   * @param[in] scale   - Scale of the format.
   */
  MediaFormat(std::string name, int crf, float bitrate, float min, float max,
              int width, int height, std::string crop, std::string scale);
  ~MediaFormat();

  std::string name;   /// @brief Name of the format.
  std::string crop;   /// @brief Crop of the format.
  std::string scale;  /// @brief Scale of the format.

  int crf;        /// @brief Constant rate factor.
  float bitrate;  /// @brief Bitrate of the format.
  float min;      /// @brief Minimum bitrate of the format.
  float max;      /// @brief Maximum bitrate of the format.
  int width;      /// @brief Width of the format.
  int height;     /// @brief Height of the format.

  /**
   * @brief Get the greatest common denominator of two numbers.
   *
   * @param[in] a - First number.
   * @param[in] b - Second number.
   *
   * @return Greatest common denominator.
   */
  static int getGCD(int, int);

  /**
   * @brief Get the resolution of the media.
   *
   * @param[in] width Width of the media.
   * @param[in] height Height of the media.
   * @param[in] newWidth New width of the media.
   *
   * @return Calculated height of the media.
   */
  static int getResolution(int width, int height, int newWidth);

  void fromCustom(int height);
};

#endif  // !MEDIA_FORMAT
