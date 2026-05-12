#include "Files.h"
#include <filesystem>
#include <iostream>

namespace fs = std::filesystem;

long long File::getsize() const {
    try {
        if (fs::exists(diskpath)) {
            return static_cast<long long>(fs::file_size(diskpath));
        }
    }
    catch (...) {}
    return 0;
}

bool File::deleteFromDisk() {
    try {
        if (fs::exists(diskpath)) {
            return fs::remove(diskpath);
        }
    }
    catch (...) {
        std::cout << "file could not be deleted\n";
    }
    return false;
}

