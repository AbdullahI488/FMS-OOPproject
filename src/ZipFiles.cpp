#include "ZipFile.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <filesystem>
#include <cstdlib>

namespace fs = std::filesystem;

ZipFile::ZipFile(const std::string& name,
    const std::string& diskPath,
    Folder* parent,
    const std::string& originalName,
    const std::string& originalType,
    const std::string& originalExtension)
    : File(name, diskPath, parent),
    originalName(originalName),
    originalType(originalType),
    originalExtension(originalExtension)
{
}

static std::string metaPath(const std::string& zipPath) {
    return zipPath + ".meta";
}

void ZipFile::writeMetaFile() const {
    std::ofstream f(metaPath(diskpath));
    if (!f.is_open()) {
        std::cerr << "[Warning] Could not write meta file for " << name << "\n";
        return;
    }

    f << "ORIGINAL_NAME=" << originalName << "\n"
        << "ORIGINAL_TYPE=" << originalType << "\n"
        << "ORIGINAL_EXT=" << originalExtension << "\n";
}

void ZipFile::readMetaFile() {
    std::ifstream f(metaPath(diskpath));
    if (!f.is_open()) return;

    std::string line;
    while (std::getline(f, line)) {
        auto sep = line.find('=');
        if (sep == std::string::npos) continue;

        std::string key = line.substr(0, sep);
        std::string value = line.substr(sep + 1);

        if (key == "ORIGINAL_NAME")
            originalName = value;
        else if (key == "ORIGINAL_TYPE")
            originalType = value;
        else if (key == "ORIGINAL_EXT")
            originalExtension = value;
    }
}

void ZipFile::createZip(const std::string& sourcePath) {
    std::string cmd = std::string("zip -r ")
        + "\"" + diskpath + "\" "
        + "\"" + sourcePath + "\""
        + " 2>/dev/null";

    int result = system(cmd.c_str());
    if (result != 0) {
        std::ofstream placeholder(diskpath);
        std::cerr << "[Warning] zip command failed. Is 'zip' installed? Created empty placeholder.\n";
    }
    else {
        std::cout << "[OK] Created " << diskpath << "\n";
    }
    writeMetaFile();
}

void ZipFile::open() {
    std::cout << "[Cannot open] Zipped files cannot be opened. "
        << "Use 'unzip " << name << "' to extract it first.\n";
}

bool ZipFile::deleteFromDisk() {
    File::deleteFromDisk();

    std::string meta = metaPath(diskpath);

    try {
        if (fs::exists(meta))
            fs::remove(meta);
        return true;
    }
    catch (const fs::filesystem_error& e) {
        std::cerr << "[Warning] Could not delete meta file: "
            << e.what() << "\n";
        return false;
    }
}

