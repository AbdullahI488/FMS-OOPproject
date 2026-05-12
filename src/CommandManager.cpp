#include "CommandManager.h"
#include "TxtFile.h"
#include "AudioFile.h"
#include "PrivateFile.h"
#include "ZipFile.h"

#include <iostream>
#include <sstream>
#include <iomanip>
#include <filesystem>
#include <vector>
#include <algorithm>

namespace fs = std::filesystem;

CommandManager::CommandManager() {
    root = new Folder("root", "root/", nullptr);
    currentfolder = root;

    registername("root");

    std::cout << "[System] File management system started.\n";
}

CommandManager::~CommandManager() {
    delete root;
}

bool CommandManager::isnametaken(const std::string& name) const 
{
   for (int i = 0; i < allnames.size(); i++)
    {
        if (allnames[i] == name)
        {
            return true;
        }
    }
    return false;
}

void CommandManager::registername(const std::string& name)
{
    allnames.push_back(name);
}

void CommandManager::unregistername(const std::string& name)
{
    for (int i = 0; i < allnames.size(); i++)
    {
        if (allnames[i] == name)
        {
            allnames.erase(allnames.begin() + i);
            return;
        }
    }
}

void CommandManager::unregistersubtree(Node* node)
{
    unregistername(node->getname());

    Folder* folder = dynamic_cast<Folder*>(node);

    if (folder != nullptr)
    {
        for (int i = 0; i < folder->getcount(); i++)
        {
            unregistersubtree(folder->getchild(i));
        }
    }
}

std::string CommandManager::getcurrentpath() const
{
    std::vector<std::string> parts;

    Folder* folder = currentfolder;

    while (folder != nullptr)
    {
        parts.push_back(folder->getname());

        folder = dynamic_cast<Folder*>(folder->getparent());
    }

    std::reverse(parts.begin(), parts.end());

    std::string path = "";

    for (int i = 0; i < parts.size(); i++)
    {
        path = path + parts[i] + " > ";
    }

    return path;
}

std::vector<std::string> CommandManager::tokenize(const std::string& line) const
{
    std::vector<std::string> tokens;

    std::istringstream stream(line);

    std::string token;

    while (stream >> token)
    {
        tokens.push_back(token);
    }

    return tokens;
}

void CommandManager::cmdls()
{
    std::cout << "\n" << std::string(55, '-') << "\n";
    std::cout << "Contents of: " << getcurrentpath() << "\n";
    std::cout << std::string(55, '-') << "\n";

    if (currentfolder->getcount() == 0)
    {
        std::cout << "  (empty folder)\n";
    }
    else
    {
        std::cout << std::left
                  << std::setw(20) << "Name"
                  << std::setw(10) << "Type"
                  << std::setw(12) << "Size"
                  << "\n";

        std::cout << std::string(42, '-') << "\n";

        for (int i = 0; i < currentfolder->getcount(); i++)
        {
            Node* child = currentfolder->getchild(i);

            long long size = child->getsize();

            std::string sizestr = "";

            if (size < 1024)
            {
                sizestr = std::to_string(size) + " B";
            }
            else if (size < 1024 * 1024)
            {
                sizestr = std::to_string(size / 1024) + " KB";
            }
            else
            {
                sizestr = std::to_string(size / (1024 * 1024)) + " MB";
            }

            std::cout << std::left
                      << std::setw(20) << child->getname()
                      << std::setw(10) << child->gettype()
                      << std::setw(12) << sizestr
                      << "\n";
        }
    }

    std::cout << std::string(55, '-') << "\n\n";
}

void CommandManager::cmdmkdir(const std::string& name)
{
    if (name.empty())
    {
        std::cout << "[Error] Usage: mkdir <name>\n";
        return;
    }

    if (isnametaken(name))
    {
        std::cout << "[Error] A node named '" << name << "' already exists.\n";
        return;
    }

    std::string path = currentfolder->getdiskpath() + name + "/";

    Folder* newfolder = new Folder(name, path, currentfolder);

    currentfolder->addchild(newfolder);

    registername(name);

    std::cout << "[OK] Folder '" << name << "' created.\n";
}

void CommandManager::cmdtouch(const std::string& type, const std::string& name)
{
    if (type.empty())
    {
        std::cout << "[Error] Usage: touch <type> <name>\n";
        return;
    }

    if (name.empty())
    {
        std::cout << "[Error] Usage: touch <type> <name>\n";
        return;
    }

    if (isnametaken(name))
    {
        std::cout << "[Error] A node named '" << name << "' already exists.\n";
        return;
    }

    if (type == "txt")
    {
        std::string path = currentfolder->getdiskpath() + name + ".txt";

        TxtFile* tf = new TxtFile(name, path, currentfolder);

        tf->createOnDisk();

        currentfolder->addchild(tf);

        registername(name);

        std::cout << "[OK] Text file created.\n";
    }
    else if (type == "audio")
    {
        std::string path = currentfolder->getdiskpath() + name + ".mp3";

        AudioFile* af = new AudioFile(name, path, currentfolder);

        af->recordAudio();

        currentfolder->addchild(af);

        registername(name);

        std::cout << "[OK] Audio file created.\n";
    }
    else if (type == "private")
    {
        std::cout << "Enter passkey: ";

        std::string passkey;

        std::getline(std::cin, passkey);

        if (passkey.empty())
        {
            std::cout << "[Error] Passkey cannot be empty.\n";
            return;
        }

        std::string path = currentfolder->getdiskpath() + name + ".priv";

        PrivateFile* pf = new PrivateFile(name, path, currentfolder, passkey);

        pf->createOnDisk();

        currentfolder->addchild(pf);

        registername(name);

        std::cout << "[OK] Private file created.\n";
    }
    else if (type == "zip")
    {
        std::cout << "[Error] Use zip command separately.\n";
    }
    else
    {
        std::cout << "[Error] Invalid file type.\n";
    }
}

void CommandManager::cmdcd(const std::string& target)
{
    if (target.empty())
    {
        std::cout << "[Error] Usage: cd <foldername>\n";
        return;
    }

    if (target == "..")
    {
        if (currentfolder == root)
        {
            std::cout << "[Info] Already at root.\n";
        }
        else
        {
            currentfolder = dynamic_cast<Folder*>(currentfolder->getparent());
        }

        return;
    }

    Node* node = currentfolder->findchild(target);

    if (node == nullptr)
    {
        std::cout << "[Error] Folder not found.\n";
        return;
    }

    Folder* folder = dynamic_cast<Folder*>(node);

    if (folder == nullptr)
    {
        std::cout << "[Error] Not a folder.\n";
        return;
    }

    currentfolder = folder;
}

void CommandManager::cmdopen(const std::string& name)
{
    if (name.empty())
    {
        std::cout << "[Error] Usage: open <filename>\n";
        return;
    }

    Node* node = currentfolder->findchild(name);

    if (node == nullptr)
    {
        std::cout << "[Error] File not found.\n";
        return;
    }

    node->open();
}
void CommandManager::cmdsearch(const std::string& name)
{
    if (name.empty())
    {
        std::cout << "[Error] Usage: search <name>\n";
        return;
    }

    std::string result = currentfolder->searchRecursive(name);//will implement after knowing if recursion and dfs allowed

    if (result == "")
    {
        std::cout << "[Not found] " << name << "\n";
    }
    else
    {
        std::cout << "[Found] " << result << "\n";
    }
}

void CommandManager::cmdrm(const std::string& name)
{
    if (name.empty())
    {
        std::cout << "[Error] Usage: rm <name>\n";
        return;
    }

    if (name == "root")
    {
        std::cout << "[Error] Cannot delete root.\n";
        return;
    }

    Node* node = currentfolder->findchild(name);

    if (node == nullptr)
    {
        std::cout << "[Error] Node not found.\n";
        return;
    }

    node->deleteFromDisk();

    unregistersubtree(node);

    currentfolder->removechild(name);

    delete node;

    std::cout << "[OK] Deleted successfully.\n";
}

void CommandManager::cmdrename(const std::string& oldname, const std::string& newname)
{
    if (oldname.empty() || newname.empty())
    {
        std::cout << "[Error] Usage: rename <oldname> <newname>\n";
        return;
    }

    if (oldname == "root")
    {
        std::cout << "[Error] Cannot rename root.\n";
        return;
    }

    if (isnametaken(newname))
    {
        std::cout << "[Error] Name already exists.\n";
        return;
    }

    Node* node = currentfolder->findchild(oldname);

    if (node == nullptr)
    {
        std::cout << "[Error] Node not found.\n";
        return;
    }

    std::string olddisk = node->getdiskpath();

    std::string newdisk = "";

    if (node->gettype() == "Folder")
    {
        newdisk = currentfolder->getdiskpath() + newname + "/";
    }
    else
    {
        std::string filename = fs::path(olddisk).filename().string();

        std::string extension = "";

        int dotindex = -1;

        for (int i = 0; i < filename.length(); i++)
        {
            if (filename[i] == '.')
            {
                dotindex = i;
            }
        }

        if (dotindex != -1)
        {
            for (int i = dotindex; i < filename.length(); i++)
            {
                extension = extension + filename[i];
            }
        }

        newdisk = currentfolder->getdiskpath() + newname + extension;
    }

    fs::rename(olddisk, newdisk);

    unregistername(oldname);

    node->setname(newname);

    node->setdiskpath(newdisk);

    registername(newname);

    std::cout << "[OK] Renamed successfully.\n";
}

void CommandManager::cmdunzip(const std::string& name)
{
    if (name.empty())
    {
        std::cout << "[Error] Usage: unzip <name>\n";
        return;
    }

    Node* node = currentfolder->findchild(name);

    if (node == nullptr)
    {
        std::cout << "[Error] Zip file not found.\n";
        return;
    }

    ZipFile* zf = dynamic_cast<ZipFile*>(node);

    if (zf == nullptr)
    {
        std::cout << "[Error] This is not a zip file.\n";
        return;
    }

    std::string origname = zf->getOriginalName();

    std::string origtype = zf->getOriginalType();

    std::string origext = zf->getOriginalExtension();

    std::string newname = origname + "-unzipped";

    if (isnametaken(newname))
    {
        std::cout << "[Error] Name already exists.\n";
        return;
    }

    std::string destdir = currentfolder->getdiskpath();

    std::string command = "unzip -o \"";
    command = command + zf->getdiskpath();
    command = command + "\" -d \"";
    command = command + destdir;
    command = command + "\"";

    system(command.c_str());

    std::string newpath = destdir + newname + origext;

    Node* newnode = nullptr;

    if (origtype == "TXT")
    {
        newnode = new TxtFile(newname, newpath, currentfolder);
    }
    else if (origtype == "Audio")
    {
        newnode = new AudioFile(newname, newpath, currentfolder);
    }
    else if (origtype == "Private")
    {
        std::cout << "Enter passkey: ";

        std::string passkey;

        std::getline(std::cin, passkey);

        newnode = new PrivateFile(newname, newpath, currentfolder, passkey);
    }
    else if (origtype == "Folder")
    {
        newnode = new Folder(newname, newpath, currentfolder);
    }
    else
    {
        newnode = new TxtFile(newname, newpath, currentfolder);
    }

    currentfolder->addchild(newnode);

    registername(newname);

    std::cout << "[OK] Unzipped successfully.\n";
}

void CommandManager::cmdhelp()
{
    std::cout << "\n";
    std::cout << "==============================\n";
    std::cout << "Available Commands\n";
    std::cout << "==============================\n";
    std::cout << "ls\n";
    std::cout << "mkdir <name>\n";
    std::cout << "touch txt <name>\n";
    std::cout << "touch audio <name>\n";
    std::cout << "touch private <name>\n";
    std::cout << "cd <name>\n";
    std::cout << "cd ..\n";
    std::cout << "open <name>\n";
    std::cout << "search <name>\n";
    std::cout << "rm <name>\n";
    std::cout << "rename <oldname> <newname>\n";
    std::cout << "unzip <name>\n";
    std::cout << "help\n";
    std::cout << "exit\n";
    std::cout << "==============================\n\n";
}
void CommandManager::run()
{
    std::string inputLine;

    while (true)
    {
        std::cout << getcurrentpath() << "$ ";

        if (!std::getline(std::cin, inputLine))
        {
            std::cout << "\n[Goodbye]\n";
            break;
        }

        if (inputLine.empty()) continue;

        std::vector<std::string> tokens = tokenize(inputLine);
        if (tokens.size() == 0) continue;

        std::string cmd = tokens[0];

        if (cmd == "exit")
        {
            std::cout << "[Goodbye]\n";
            break;
        }
        else if (cmd == "ls") cmdls();
        else if (cmd == "mkdir") cmdmkdir(tokens.size() > 1 ? tokens[1] : "");
        else if (cmd == "touch")
            cmdtouch(
                tokens.size() > 1 ? tokens[1] : "",
                tokens.size() > 2 ? tokens[2] : ""
            );
        else if (cmd == "cd") cmdcd(tokens.size() > 1 ? tokens[1] : "");
        else if (cmd == "open") cmdopen(tokens.size() > 1 ? tokens[1] : "");
        else if (cmd == "search") cmdsearch(tokens.size() > 1 ? tokens[1] : "");
        else if (cmd == "rm") cmdrm(tokens.size() > 1 ? tokens[1] : "");
        else if (cmd == "rename")
            cmdrename(
                tokens.size() > 1 ? tokens[1] : "",
                tokens.size() > 2 ? tokens[2] : ""
            );
        else if (cmd == "unzip") cmdunzip(tokens.size() > 1 ? tokens[1] : "");
        else if (cmd == "help") cmdhelp();
        else std::cout << "[Error] Unknown command\n";
    }
}
