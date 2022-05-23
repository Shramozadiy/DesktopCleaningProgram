#include "Desktop.h"

Desktop& Desktop::getInstance()
{
    static Desktop  instance;
    return instance;
}

void Desktop::initDesktop()
{
    userDesktopPath = std::string(getenv("USERPROFILE")) + "\\Desktop\\";
    publicDesktopPath = std::string("C:\\Users\\Public\\Desktop\\");
}

std::string Desktop::getUserDesktopPath()
{
    return userDesktopPath;
}

std::string Desktop::getPublicDesktopPath()
{
    return publicDesktopPath;
}

std::list <std::string> Desktop::getFileNames(std::string path)
{
    std::list <std::string> fileNames;

    WIN32_FIND_DATAA wfd;

    HANDLE const hFind = FindFirstFileA((path + "*").c_str(), &wfd);

    if (INVALID_HANDLE_VALUE != hFind)
    {
        do
        {
            fileNames.push_back(&wfd.cFileName[0]);

        } while (NULL != FindNextFileA(hFind, &wfd));

        FindClose(hFind);
    }

    // Удаляем из списка папки \., \.., \desktop.ini
    fileNames.erase(
        std::remove(fileNames.begin(), fileNames.end(), "."), fileNames.end());
    fileNames.erase(
        std::remove(fileNames.begin(), fileNames.end(), ".."), fileNames.end());
    fileNames.erase(
        std::remove(fileNames.begin(), fileNames.end(), "desktop.ini"), fileNames.end());

    return fileNames;
}

std::list <std::string> Desktop::getFileNames()
{
    std::list <std::string> fileNames;

    std::list <std::string> path = {
        getUserDesktopPath(),
        getPublicDesktopPath()};

    for (auto i : path)
    {
        std::list <std::string> list = getFileNames(i);
        fileNames.insert(fileNames.end(), list.begin(), list.end());
    }
    
    return fileNames;
}

std::list <std::string> Desktop::getFilePaths()
{
    std::list <std::string> filePaths;

    std::list <std::string> path = {
        getUserDesktopPath(),
        getPublicDesktopPath() };

    for (auto i : path)
    {
        std::list <std::string> list = getFileNames(i);
       
        for (auto& j: list)
        {
            j = i + j;
        }

        filePaths.insert(filePaths.end(), list.begin(), list.end());
    }

    return filePaths;
}

std::list <std::string> Desktop::getFilePaths(std::string path, std::list <std::string> fileNames)
{
    std::list <std::string> filePaths;

    for (auto i : fileNames)
    {
        filePaths.push_back(path + i);
    }
    return filePaths;
}

bool Desktop::isInFolder(std::string path, std::string fileName)
{
    std::list <std::string> files;
    files = getFileNames(path);

    for (auto i : files)
    {
        if (i == fileName)
            return 1;
    }

    return 0;
}