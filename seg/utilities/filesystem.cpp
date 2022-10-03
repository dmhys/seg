#include "seg/utilities/filesystem.h"

#include <dirent.h>

#include <algorithm>
#include <cstring>
#include <stdexcept>
#include <string>
#include <vector>

namespace seg {
std::vector<std::string> getFileList(const std::string &directory, std::string extension)
{
    std::vector<std::string> output;
    DIR *dir = nullptr;
    struct dirent *entry = nullptr;

    const bool do_filter = extension != "" && extension != ".";
    if (extension.front() != '.') extension = "." + extension;

    dir = opendir(directory.c_str());
    if (dir == nullptr) throw std::invalid_argument("No such directory.");

    while ((entry = readdir(dir)) != nullptr) {
        if (entry->d_name[0] == '.') continue;

        if (do_filter) {
            char *ptr = strrchr(entry->d_name, '.');
            if (ptr == nullptr) continue;

            if (strcmp(ptr, extension.data()) != 0) continue;
        }

        output.push_back(std::string(entry->d_name));
    }
    closedir(dir);

    std::sort(output.begin(), output.end());

    return output;
}

}  // namespace seg