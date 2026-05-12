#include"Node.h"
using namespace std;
Node::Node(string n,string p,Folder* np) {
	Node::diskpath = p;
	Node::name = n;
	Node::parent = np;
}
//no need to write getters setters here since they are in header
