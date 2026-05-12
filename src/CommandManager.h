#ifndef COMMANDMANAGER_H
#define COMMANDMANAGER_H

#include "Folder.h"

#include <string>
#include <unordered_set>
#include <vector>

class CommandManager
{
private:

    Folder* root;
    Folder* currentfolder;

    std::unordered_set<std::string> allnames;

    bool isnametaken(const std::string& name) const;

    void registername(const std::string& name);

    void unregistername(const std::string& name);

    void unregistersubtree(Node* node);

    std::string getcurrentpath() const;

    std::string buildchildpath(const std::string& extension = "") const;

    void cmdls();

    void cmdmkdir(const std::string& name);

    void cmdtouch(const std::string& type, const std::string& name);

    void cmdcd(const std::string& target);

    void cmdopen(const std::string& name);

    void cmdsearch(const std::string& name);

    void cmdrm(const std::string& name);

    void cmdrename(const std::string& oldname, const std::string& newname);

    void cmdunzip(const std::string& name);

    void cmdhelp();

    std::vector<std::string> tokenize(const std::string& line) const;

public:

    CommandManager();

    ~CommandManager();

    void run();
};

#endif
