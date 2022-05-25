#include "MoveToRepository.h"



long long getLostTime(SYSTEMTIME t1, SYSTEMTIME t2)
{
    long long lostTime = 0; // В минутах

    lostTime += t1.wMinute - t2.wMinute;
    lostTime += (t1.wHour - t2.wHour) * 60;
    lostTime += (t1.wDay - t2.wDay) * 60 * 24;
    lostTime += (t1.wMonth - t2.wMonth) * 60 * 24 * 30;
    lostTime += (t1.wYear - t2.wYear) * 60 * 24 * 365;

    return lostTime;
}

bool getLastOpenTimeFile(std::string path, long long time)
{
    WIN32_FILE_ATTRIBUTE_DATA fileInfo;
    GetFileAttributesExA(path.c_str(), GetFileExInfoStandard, (void*)&fileInfo);

    FILETIME fa = fileInfo.ftLastAccessTime;

    SYSTEMTIME stUTC, stLocal, stNow;
    GetSystemTime(&stNow);
    FileTimeToSystemTime(&fa, &stUTC);
    SystemTimeToTzSpecificLocalTime(NULL, &stUTC, &stLocal);
    SystemTimeToTzSpecificLocalTime(NULL, &stNow, &stNow);

    long long lostTime = getLostTime(stNow, stLocal);

    if (lostTime >= time)
        return 1;
    return 0;
}

void moveFilesToRepository()
{
    // Создание папки если ее не существует
    std::string userDesktopPath = Desktop::getInstance().getUserDesktopPath();
    std::string repository = Data::getInstance().getRepository();
    std::list <std::string> exceptions = Data::getInstance().getExceptions();
    std::string command = "mkdir \"" + userDesktopPath + repository + "\"";
    system(command.c_str());

    // Получить пути файлов
    std::list <std::string> filePaths = Desktop::getInstance().getFilePaths();

    // Получить названия файлов
    std::list <std::string> fileNames = Desktop::getInstance().getFileNames();

    // Получить список путей исключений
    std::list <std::string> exceptionsPath = Desktop::getInstance().getFilePaths(userDesktopPath, exceptions);

    for (auto& i : fileNames)
    {

        auto it = find(exceptions.begin(), exceptions.end(), i);

        if (it == exceptions.end())
        {
            std::string newName;
            if (Desktop::getInstance().isInFolder(userDesktopPath + repository + "\\", i))
            {
                newName = i;
                int num = 1;
                while (Desktop::getInstance().isInFolder(userDesktopPath + repository + "\\", newName))
                {
                    newName = i;
                    auto it = find(newName.begin(), newName.end(), '.');
                    newName.insert((size_t)(it - newName.begin()), "(" + std::to_string(num) + ")");
                    num++;
                }
                newName = "\\" + newName;
            }

            i = newName;
        }

    }


    //for (auto& i : files)
    auto fileName = fileNames.begin();
    auto filePath = filePaths.begin();

    for (auto& i : filePaths)
    {

        auto it = find(exceptionsPath.begin(), exceptionsPath.end(), i);

        if (it == exceptionsPath.end())
        {
            if (getLastOpenTimeFile(*filePath, Data::getInstance().getFileAgeLimits()) == 1)
            {
                std::string command = "move \"" + *filePath + "\" \"" + userDesktopPath + repository + *fileName + "\"";
                system(command.c_str());
            }
        }

        filePath++;
        fileName++;
    }
}
