#ifndef TXTFILE_H
#define TXTFILE_H
#include "Files.h"
#include <vector>
#include <string>

class TxtFile : public File {
protected:
    std::vector<std::string> lines;

    void loadFromDisk() override;
    void saveToDisk() override;
    void runEditor() override;

public:
    TxtFile(const std::string& name,
        const std::string& diskPath,
        Folder* parent);

    void createOnDisk() override;
    void open() override;
    std::string getType() const override { return "TXT"; }

    bool deleteFromDisk() override;
};

#endif
