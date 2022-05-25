#include "Data.h"


Data& Data::getInstance()
{
    static Data  instance;
    return instance;
}

void Data::loadData(std::string fileName)
{
    std::ifstream in(fileName);
    if (in.is_open())
    {
        std::string buffer;
        size_t count;
        // Получаем размер списка исключений
        in >> count;

        // Загружаем список исключений
        for (int i = 0; i < count; i++)
        {
            getline(in, buffer);
            exceptions.push_back(buffer);
        }

        // Загружаем хранилище
        getline(in, buffer);
        repository = buffer;
    }
    in.close();     // закрываем файл
}

void Data::saveData(std::string fileName)
{
    std::ofstream out(fileName);
    if (out.is_open())
    {
        // Записываем размер списка исключений
        out << exceptions.size();

        // Записываем список исключений
        for (auto i : exceptions)
        {
            out << i << std::endl;
        }

        // Записываем хранилище
        out << repository << std::endl;
    }
    out.close();
}

std::list <std::string> Data::getExceptions()
{
    return exceptions;
}

void Data::addException(std::string fileName)
{
    exceptions.erase(std::remove(exceptions.begin(), exceptions.end(), fileName), exceptions.end());
    exceptions.push_back(fileName);
}

void Data::deleteException(std::string fileName)
{
    exceptions.erase(std::remove(exceptions.begin(), exceptions.end(), fileName), exceptions.end());
}

std::string Data::getRepository()
{
   
    return repository;
}

void Data::setRepository(std::string fileName)
{
    repository = fileName;
}

void Data::setFileAgeLimits(long long minutes)
{
    fileAgeLimits = minutes;
}

long long Data::getFileAgeLimits()
{
    return fileAgeLimits;
}