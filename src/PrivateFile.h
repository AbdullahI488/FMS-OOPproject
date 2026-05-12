#ifndef PRIVATEFILE_H
#define PRIVATEFILE_H

#include "TxtFile.h"
#include <string>

class PrivateFile : public TxtFile {
private:
    std::string passkey;
    static const std::string PREFIX;

    bool verifyPasskey() const;

public:
    PrivateFile(const std::string& name,
        const std::string& diskPath,
        Folder* parent,
        const std::string& passkey);

    void createOnDisk();

    void loadFromDisk() override;
    void saveToDisk() override;

    void open() override;
    std::string getType() const override { return "Private"; }

    bool deleteFromDisk() override;
};

#endif
