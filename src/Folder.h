#ifndef FOLDER_H
#define FOLDER_H

#include "Node.h"

class Folder : public Node {

	Node** space;
	int count;
	int capacity;
	void resize();

public:

	Folder(std::string name, std::string path, Folder* ptr);

	~Folder();

	void addchild(Node* node);

	bool removechild(std::string name);

	Node* findchild(std::string name) const;

	std::string searchrecursive(std::string targetname) const;

	int getcount() const {
		return count;
	}

	Node* getchild(int i) const {
		return space[i];
	}

	void open() override;

	std::string gettype() const override {
		return "Folder";
	}

	long long getsize() const override;

	void deletefromdisk() override;
};

#endif
