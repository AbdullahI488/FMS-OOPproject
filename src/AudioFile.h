#ifndef AUDIOFILE_H
#define AUDIOFILE_H

#include "Files.h"
#include <string>

class AudioFile : public File {
private:
    std::string buildRecordCommand() const;
    std::string buildPlayCommand() const;

public:
    AudioFile(const std::string& name,
        const std::string& diskPath,
        Folder* parent);

    void recordAudio();

    void open() override;
    std::string getType() const override { return "Audio"; }
};

#endif

