#include "AudioFile.h"

#include <iostream>
#include <fstream>
#include <stdexcept>
#include <thread>
#include <chrono>
#include <cstdlib>

AudioFile::AudioFile(const std::string& name,
    const std::string& diskPath,
    Folder* parent)
    : File(name, diskPath, parent)
{
}

std::string AudioFile::buildRecordCommand() const {
    std::string cmd = "ffmpeg -y ";

#if defined(_WIN32)
    cmd += "-f dshow -i audio=\"Microphone\" ";
#elif defined(__APPLE__)
    cmd += "-f avfoundation -i \":0\" ";
#else
    cmd += "-f alsa -i default ";
#endif

    cmd += "-t 5 -ac 1 -ar 44100 ";
    cmd += "\"" + diskpath + "\"";
    cmd += " 2>/dev/null";

    return cmd;
}

std::string AudioFile::buildPlayCommand() const {
    std::string cmd;

#if defined(_WIN32)
    cmd = "start \"\" \"" + diskpath + "\"";
#elif defined(__APPLE__)
    cmd = "afplay \"" + diskpath + "\"";
#else
    cmd = "ffplay -nodisp -autoexit \"" + diskpath + "\" 2>/dev/null";
#endif

    return cmd;
}

void AudioFile::recordAudio() {
    std::cout << "\n[AudioFile] Starting microphone recording...\n";
    std::cout << "Recording: ";
    std::cout.flush();

    std::thread countdownThread([]() {
        for (int i = 5; i >= 1; --i) {
            std::cout << i << "... ";
            std::cout.flush();
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }
    });

    std::string cmd = buildRecordCommand();
    int result = system(cmd.c_str());

    countdownThread.join();

    std::cout << "\n";

    if (result != 0) {
        std::ofstream placeholder(diskpath);

        std::cerr << "[Warning] ffmpeg recording failed (is ffmpeg installed?).\n"
                  << "Created empty placeholder: " << diskpath << "\n";
    }
    else {
        std::cout << "[OK] Audio recorded: " << diskpath << "\n";
    }
}

void AudioFile::open() {
    std::cout << "[Playing] " << name << "\n";

    std::string cmd = buildPlayCommand();
    int result = system(cmd.c_str());

    if (result != 0) {
        std::cerr << "[Error] Could not play audio. Is ffplay/afplay installed?\n";
    }
    else {
        std::cout << "[Done]\n";
    }
}

