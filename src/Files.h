#ifndef FILES_H
#define FILES_H
#include<iostream>
#include "Node.h"
class Files {
public:

	Files(std::string n, std::string dp, Folder* np);

	long long getsize() const override;

	void deletefromdisk() override;

	virtual void open() = 0;

	virtual std::string gettype() const = 0;

	virtual ~Files() = default;

};
#endif
