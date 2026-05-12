#include "Folder.h"
#include <iostream>
#include <filesystem>//for actual filemaking

namespace fs = std::filesystem;

Folder::Folder(std::string name, std::string path, Folder* ptr)
    : Node(name, path, ptr) {
    space = new Node*[5];
    count = 0;
    capacity = 5;

    try {
        fs::create_directories(path);
    }
    catch (...) {
        std::cout << "folder could not be created\n";
    }
}

void Folder::resize() {
    int newcapacity = capacity * 2;
    Node** newspace = new Node*[newcapacity];

    for (int i = 0; i < count; i++) {
        newspace[i] = space[i];
    }

    delete[] space;

    space = newspace;
    capacity = newcapacity;
}

void Folder::addchild(Node* node) {
    if (count == capacity) {
        resize();
    }
    space[count++] = node;
}

bool Folder::removechild(const std::string& name) {
    for (int i = 0; i < count; i++) {
        if (space[i]->getname() == name) {
            for (int j = i; j < count - 1; j++) {
                space[j] = space[j + 1];
            }
            count--;
            return true;
        }
    }
    return false;
}

Node* Folder::findchild(const std::string& name) const {
    for (int i = 0; i < count; i++) {
        if (space[i]->getname() == name) {
            return space[i];
        }
    }
    return nullptr;
}

std::string Folder::searchRecursive(const std::string& targetname) const {
    if (getname() == targetname) {
        return getdiskpath();
    }

    for (int i = 0; i < count; i++) {
        if (space[i]->getname() == targetname) {
            return space[i]->getdiskpath();
        }

        Folder* folder = dynamic_cast<Folder*>(space[i]);
        if (folder != nullptr) {
            std::string result = folder->searchRecursive(targetname);
            if (!result.empty()) {
                return result;
            }
        }
    }

    return "";
}

long long Folder::getsize() const {
    long long total = 0;

    for (int i = 0; i < count; i++) {
        total += space[i]->getsize();
    }

    return total;
}

bool Folder::deleteFromDisk() {
    try {
        if (fs::exists(diskpath)) {
            fs::remove_all(diskpath);
            return true;
        }
    }
    catch (...) {
        std::cout << "folder could not be deleted\n";
    }
    return false;
}

Folder::~Folder() {
    for (int i = 0; i < count; i++) {
        delete space[i];
    }

    delete[] space;
}

void Folder::open() {
}
