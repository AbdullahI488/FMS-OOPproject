#include "TXTFile.h"

#include <iostream>
#include <fstream>
#include <stdexcept>
#include <cstdio>

TxtFile::TxtFile(const std::string& name,
    const std::string& diskPath,
    Folder* parent)
    : File(name, diskPath, parent)
{
}

void TxtFile::createOnDisk() {
    std::ofstream f(diskpath);
    if (!f.is_open()) {
        throw std::runtime_error("Could not create file: " + diskpath);
    }
    std::cout << "[OK] Created " << diskpath << "\n";
}

void TxtFile::loadFromDisk() {
    lines.clear();
    std::ifstream f(diskpath);
    if (!f.is_open()) {
        lines.push_back("");
        return;
    }

    std::string line;
    while (std::getline(f, line)) {
        lines.push_back(line);
    }

    if (lines.empty()) lines.push_back("");
}

void TxtFile::saveToDisk() {
    std::ofstream f(diskpath, std::ios::trunc);
    if (!f.is_open()) {
        throw std::runtime_error("Could not save file: " + diskpath);
    }

    for (size_t i = 0; i < lines.size(); ++i) {
        f << lines[i];
        if (i + 1 < lines.size()) f << "\n";
    }

    std::cout << "[Saved]\n";
}

void TxtFile::runEditor() {
    int cursor = 0;
    std::string cmd;

    while (true) {
        std::cout << "\n--- " << name << " ---\n";

        for (size_t i = 0; i < lines.size(); i++) {
            if (static_cast<int>(i) == cursor) std::cout << "> ";
            else std::cout << "  ";

            std::cout << i + 1 << ": " << lines[i] << std::endl;
        }

        std::cout << "\nCommands: up, down, edit, add, del, close\n> ";
        std::getline(std::cin, cmd);

        if (cmd == "up") {
            if (cursor > 0) cursor--;
        }
        else if (cmd == "down") {
            if (cursor < static_cast<int>(lines.size()) - 1) cursor++;
        }
        else if (cmd == "edit") {
            std::cout << "Enter new text: ";
            std::getline(std::cin, lines[cursor]);
        }
        else if (cmd == "add") {
            std::string text;
            std::cout << "Enter new line: ";
            std::getline(std::cin, text);

            lines.insert(lines.begin() + cursor + 1, text);
            cursor++;
        }
        else if (cmd == "del") {
            if (lines.size() > 1) {
                lines.erase(lines.begin() + cursor);
                if (cursor >= static_cast<int>(lines.size())) {
                    cursor = static_cast<int>(lines.size()) - 1;
                }
            }
        }
        else if (cmd == "close") {
            break;
        }
        else {
            std::cout << "Invalid command\n";
        }
    }

    saveToDisk();
}

void TxtFile::open() {
    try {
        loadFromDisk();
        runEditor();
    }
    catch (const std::exception& e) {
        std::cout << "[Error opening file] " << e.what() << "\n";
    }
}

bool TxtFile::deleteFromDisk() {
    std::ifstream file(diskpath);
    if (!file.good()) {
        std::cout << "[Error] File does not exist: " << diskpath << "\n";
        return false;
    }

    file.close();
    if (std::remove(diskpath.c_str()) == 0) {
        std::cout << "[OK] File deleted: " << diskpath << "\n";
        return true;
    }

    std::cout << "[Error] Could not delete file: " << diskpath << "\n";
    return false;
}

