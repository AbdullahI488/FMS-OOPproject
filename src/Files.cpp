#include "Files.h"
#include <filesystem>
#include <iostream>

namespace fs = std::filesystem;

Files::Files(std::string name, std::string dp, Folder* np) : Node(name, path, ptr) {}

long long Files::getsize() const {
	try {

		if (fs::exists(pathname)) {
			return (long long)fs::file_size(pathname);
		}
	}

	catch (...) {}

	return 0;
}

bool Files::deletefromdisk() {//actual file deletion

	try {
		if (fs::exists(pathname)) {
			fs::remove(pathname);
			return true;
		}
	}

	catch (...) {
		std::cout << "file could not be deleted\n";
		return false;
	}
}
