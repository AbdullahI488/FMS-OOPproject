#ifndef FILE_H
#define FILE_H

#include <string>

class Folder;

class File {
protected:
    std::string name;
    std::string diskPath;
    Folder* parent;

public:
    File(const std::string& name,
        const std::string& diskPath,
        Folder* parent)
        : name(name), diskPath(diskPath), parent(parent) {
    }

    virtual ~File() = default;

    const std::string& getName() const { return name; }
    const std::string& getDiskPath() const { return diskPath; }
    Folder* getParent() const { return parent; }

    virtual void createOnDisk()=0;
    virtual void open() = 0;
    virtual std::string getType() const = 0;
    virtual bool deleteFromDisk() = 0;
    virtual void loadFromDisk()=0;
     virtual void saveToDisk()=0;
     virtual void runEditor() = 0;
};

#endif
