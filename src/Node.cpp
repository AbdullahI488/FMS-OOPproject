#include "Node.h"

Node::Node(const std::string& n, const std::string& dp, Folder* np)
    : name(n), diskpath(dp), parent(np) {
}

//no need to write getters setters here since they are in header
