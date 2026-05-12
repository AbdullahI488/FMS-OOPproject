#include"TXTFile.h";
using namespace std;

#include <iostream>
#include <fstream>
#include <stdexcept>



TxtFile::TxtFile(const std::string& name,
    const std::string& diskPath,
    Folder* parent)
    : File(name, diskPath, parent)
{
}


void TxtFile::createOnDisk() {
    std::ofstream f(diskPath);
    if (!f.is_open()) {
        throw std::runtime_error("Could not create file: " + diskPath);
    }
    // File is created empty; f closes automatically when it goes out of scope
    std::cout << "[OK] Created " << diskPath << "\n";
}


void TxtFile::loadFromDisk() {
    lines.clear();
    std::ifstream f(diskPath);
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
    std::ofstream f(diskPath, std::ios::trunc);
    if (!f.is_open()) {
        throw std::runtime_error("Could not save file: " + diskPath);
    }
    for (size_t i = 0; i < lines.size(); ++i) {
        f << lines[i];
        if (i + 1 < lines.size()) f << "\n";   
    }
    std::cout << "[Saved]\n";
}

void TxtFile::runEditor() {
    int cursor = 0;
    string cmd;

    while (true) {
        cout << "\n--- " << name << " ---\n";

        for (int i = 0; i < lines.size(); i++) {
            if (i == cursor) cout << "> ";
            else cout << "  ";

            cout << i + 1 << ": " << lines[i] << endl;
        }
        cout << "\nCommands: up, down, edit, add, del, close\n> ";
        getline(cin, cmd);

        if (cmd == "up") {
            if (cursor > 0)
                cursor--;
        }

        else if (cmd == "down") {
            if (cursor < lines.size() - 1)
                cursor++;
        }

        else if (cmd == "edit") {
            cout << "Enter new text: ";
            getline(cin, lines[cursor]);
        }

        else if (cmd == "add") {
            string text;
            cout << "Enter new line: ";
            getline(cin, text);

            lines.insert(lines.begin() + cursor + 1, text);
            cursor++;
        }

        else if (cmd == "del") {
            if (lines.size() > 1) {
                lines.erase(lines.begin() + cursor);

                if (cursor >= lines.size())
                    cursor = lines.size() - 1;
            }
        }
        else if (cmd == "close") {
            break;
        }
        else {
            cout << "Invalid command\n";
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
    std::ifstream file(diskPath);

    if (!file.good()) {
        std::cout << "[Error] File does not exist: " << diskPath << "\n";
        return false;
    }

    file.close();

    if (remove(diskPath.c_str()) == 0) {
        std::cout << "[OK] File deleted: " << diskPath << "\n";
        return true;
    }

    std::cout << "[Error] Could not delete file: " << diskPath << "\n";
    return false;
}

