#ifndef NODE_H
#define NODE_H
#include<iostream>
class Folder;
class Node {
protected:
	std::string name;
	std::string pathname;
	Folder* ptr;
public:
	Node(std::string n, std::string p, Folder* ptr);
	std::string GetName();
	std::string GetPath();
	Folder* GetParent();
	virtual ~Node();
	virtual std::string GetType();



};

#endif
