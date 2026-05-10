#include"Node.h"
using namespace std;
Node::Node(string n,string p,Folder*ptr) {
	Node::pathname = p;
	Node::name = n;
	Node::ptr = ptr;
}
string Node::GetName() {
	return name;
}
string Node::GetPath() {
	return pathname;
}
Folder* Node::GetParent() {
	return ptr;
}
