#pragma once

#include <Windows.h>
#include <string>
#include <list>

#include "Desktop.h"
#include "Data.h"

long long getLostTime(SYSTEMTIME t1, SYSTEMTIME t2);

bool getLastOpenTimeFile(std::string path, long long time);

void moveFilesToRepository();