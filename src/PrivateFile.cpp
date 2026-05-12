#include "PrivateFile.h"
#include "Files.h"
#include <iostream>
#include <fstream>
#include <stdexcept>

const std::string PrivateFile::PREFIX = "PASSKEY:";

PrivateFile::PrivateFile(const std::string& name,
    const std::string& diskPath,
    Folder* parent,
    const std::string& passkey)
    : TxtFile(name, diskPath, parent),
    passkey(passkey)
{
}

void PrivateFile::createOnDisk() {
    std::ofstream f(diskpath);
    if (!f.is_open()) {
        throw std::runtime_error("Could not create file: " + diskpath);
    }
    f << PREFIX << passkey << "\n";
    std::cout << "[OK] Created private file: " << diskpath << "\n";
}

void PrivateFile::loadFromDisk() {
    lines.clear();
    std::ifstream f(diskpath);
    if (!f.is_open()) {
        lines.push_back("");
        return;
    }

    std::string firstLine;
    if (std::getline(f, firstLine)) {
        if (firstLine.rfind(PREFIX, 0) == 0) {
            passkey = firstLine.substr(PREFIX.size());
        }
    }

    std::string line;
    while (std::getline(f, line)) {
        lines.push_back(line);
    }

    if (lines.empty()) lines.push_back("");
}

void PrivateFile::saveToDisk() {
    std::ofstream f(diskpath, std::ios::trunc);
    if (!f.is_open()) {
        throw std::runtime_error("Could not save file: " + diskpath);
    }

    f << PREFIX << passkey << "\n";

    for (size_t i = 0; i < lines.size(); ++i) {
        f << lines[i];
        if (i + 1 < lines.size()) f << "\n";
    }

    std::cout << "[Saved]\n";
}

bool PrivateFile::verifyPasskey() const {
    std::cout << "Enter passkey: ";
    std::string input;
    std::getline(std::cin, input);
    return input == passkey;
}

void PrivateFile::open() {
    std::cout << "[Private File] " << name << " requires a passkey.\n";

    for (int attempt = 1; attempt <= 3; ++attempt) {
        if (verifyPasskey()) {
            try {
                loadFromDisk();
                runEditor();
            }
            catch (const std::exception& e) {
                std::cerr << "[Error] " << e.what() << "\n";
            }
            return;
        }
        std::cout << "[Wrong passkey] Attempt " << attempt << " of 3.\n";
    }

    std::cout << "[Access denied] Too many failed attempts.\n";
}

bool PrivateFile::deleteFromDisk() {
    std::cout << "[Private File] Deletion requires passkey verification.\n";

    if (!verifyPasskey()) {
        std::cout << "[Cancelled] Incorrect passkey.\n";
        return false;
    }

    std::cout << "Are you sure you want to permanently delete '" << name
              << "'? (yes/no): ";

    std::string confirm;
    std::getline(std::cin, confirm);

    if (confirm == "yes") {
        File::deleteFromDisk();
        std::cout << "[Deleted] " << name << "\n";
        return true;
    }

    std::cout << "[Cancelled]\n";
    return false;
}
