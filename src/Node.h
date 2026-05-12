#ifndef NODE_H
#define NODE_H

#include <string>

class Folder;

class Node {

protected:
    std::string name;
    std::string diskpath;
    Folder* parent; //it is a ptr to parent so instead var named ptr it should be name parent

public:
    Node(const std::string& n, const std::string& dp, Folder* np);
    virtual ~Node() = default;

    const std::string& getname() const {
        return name;
    }

    const std::string& getdiskpath() const {
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
    virtual bool deleteFromDisk() = 0;
};

#endif
