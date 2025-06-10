#ifndef DIRECTORY_UTILS_H
#define DIRECTORY_UTILS_H

#include <filesystem>
#include <string>
#include <vector>

class DirectoryUtils {
public:
  static std::vector<std::filesystem::directory_entry> getFilesInCWD(void);

  static std::vector<std::filesystem::directory_entry> getFilesInDirectory(std::string path);
  static std::vector<std::filesystem::directory_entry> getFilesInDirectory(
    std::string path, std::string ext);
  static std::vector<std::filesystem::directory_entry> getFilesInDirectory(
    std::string path, std::vector<const char*> exts);
  static std::vector<std::filesystem::directory_entry> getFilesInCWDWithExt(
    std::string);
  static std::vector<std::filesystem::directory_entry> getFilesInCWDWithExt(
    std::vector<const char*>);
  static std::vector<std::filesystem::directory_entry> findFileInSubdir(
    std::string);
  static bool createDir(std::string path);
  static bool createDir(std::string path, bool recrsive);
};

#endif  // !DIRECTORY_UTILS_H
