#ifndef NODE_H
#define NODE_H

#include <string>
#include <iostream>

class Folder;

class Node {

protected:

    std::string name;
    std::string diskpath;
    Folder* parent; //it is a ptr to a parent so this var name makes more sense

public:

    Node(const std::string& n, const std::string& dp, Folder* np); 

    virtual ~Node() = default;

    std::string getname() const {
        return name;
    }

    std::string getdiskpath() const {
        return diskpath;
    }

    Folder* getparent() const {
        return parent;
    }

    void setname(const std::string& n) {
        name = n;
    }

    void setdiskpath(const std::string& dp) {
        diskpath = dp;
    }

    virtual void open() = 0; 

    virtual std::string gettype() const = 0;

    virtual long long getsize() const = 0;

    virtual void deletefromdisk() = 0;
};

#endif
