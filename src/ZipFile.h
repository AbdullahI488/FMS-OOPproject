#ifndef ZIPFILES_H
#define ZIPFILES_H

#include "Files.h"
#include <string>

class ZipFile : public File {
private:
    std::string originalName;
    std::string originalType;
    std::string originalExtension;

    void writeMetaFile() const;
    void readMetaFile();

public:
    ZipFile(const std::string& name,
        const std::string& diskPath,
        Folder* parent,
        const std::string& originalName,
        const std::string& originalType,
        const std::string& originalExtension);

    void createZip(const std::string& sourcePath);

    std::string getOriginalName() const { return originalName; }
    std::string getOriginalType() const { return originalType; }
    std::string getOriginalExtension() const { return originalExtension; }

    void open() override;
    std::string getType() const override { return "ZIP"; }

    bool deleteFromDisk() override;
};

#endif
