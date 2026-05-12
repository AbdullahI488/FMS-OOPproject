#ifndef TXTFILE_H
#define TXTFILE_H
#include "Files.h"
#include <vector>
#include <string>

class TxtFile : public File {
protected:
    std::vector<std::string> lines;

    virtual void loadFromDisk();
    virtual void saveToDisk();
    virtual void runEditor();

public:
    TxtFile(const std::string& name,
        const std::string& diskPath,
        Folder* parent);

    virtual void createOnDisk();
    void open() override;
    std::string gettype() const override { return "TXT"; }

    bool deleteFromDisk() override;
};

#endif
