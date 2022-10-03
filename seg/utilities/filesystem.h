#ifndef SEG_UITILITES_FILESYSTEM_H
#define SEG_UITILITES_FILESYSTEM_H

#include <string>
#include <vector>

// some convinient functions for beginers.
// C++ >=17 users can use std::filesystems instead.
namespace seg {
std::vector<std::string> getFileList(const std::string &directory,
                                     std::string extension = "");
}

#endif