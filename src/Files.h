#ifndef FILES_H
#define FILES_H

#include "Node.h"
#include <string>

class File : public Node {
public:
    File(const std::string& name,
         const std::string& diskPath,
         Folder* parent)
        : Node(name, diskPath, parent) {
    }

    virtual ~File() = default;

    virtual long long getsize() const override;
    virtual bool deleteFromDisk() override;
};

#endif
