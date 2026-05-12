#include "File.h"
#include <filesystem>
#include <iostream>

namespace fs = std::filesystem;

File::File(std::string name, std::string dp, Folder* np) : Node(name, path, ptr) {}

long long File::getsize() const {
	try {

		if (fs::exists(pathname)) {
			return (long long)fs::file_size(pathname);
		}
	}

	catch (...) {}

	return 0;
}

void File::deletefromdisk() {//actual file deletion

	try {
		if (fs::exists(pathname)) {
			fs::remove(pathname);
		}
	}

	catch (...) {
		std::cout << "file could not be deleted\n";
	}
}
