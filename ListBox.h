#pragma once

#include <Windows.h>
#include <list>
#include <string>

std::string getTextFromList(HWND hWnd, int IDC);

std::list <std::string> getListText(HWND hWnd, int IDC);

void refreshList(HWND hWnd, int IDC, std::list <std::string> list);

void setTextToList(HWND hWnd, int IDC, std::list <std::string> list);