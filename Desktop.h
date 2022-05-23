#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include <Windows.h>
#include <string>
#include <list>


class Desktop
{
    Desktop() {}
    Desktop(const Desktop&);
    Desktop& operator=(Desktop&);

    std::string userDesktopPath;
    std::string publicDesktopPath;
public:


    static Desktop& getInstance();

    void initDesktop();

    std::string getUserDesktopPath();

    std::string getPublicDesktopPath();

    std::list<std::string> getFileNames(std::string path);

    std::list <std::string> getFileNames();

    std::list<std::string> getFilePaths();

    std::list<std::string> getFilePaths(std::string path, std::list<std::string> fileNames);

    bool isInFolder(std::string path, std::string fileName);


};
