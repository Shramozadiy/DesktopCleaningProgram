#include "ListBox.h"


std::string getTextFromList(HWND hWnd, int IDC)
{
    HWND hList = GetDlgItem(hWnd, IDC);
    char str[256];
    int i = SendMessage(hList, LB_GETCURSEL, 0, 0);
    SendMessageA(hList, LB_GETTEXT, i, (LPARAM)str);
    SendMessage(hList, LB_SETSEL, 0, -1);

    return str;
}


std::list <std::string> getListText(HWND hWnd, int IDC)
{
    HWND hList = GetDlgItem(hWnd, IDC);
    std::list <std::string> list;
    int Index[256] = {};
    int i = SendMessage(hList, LB_GETSELCOUNT, 0, 0);
    SendMessageA(hList, LB_GETSELITEMS, i, (LPARAM)Index);
    for (int j = 0; j < i; j++)
    {
        char str[256];
        SendMessageA(hList, LB_GETTEXT, Index[j], (LPARAM)str);
        list.push_back(str);
    }

    SendMessage(hList, LB_SETSEL, 0, -1);

    return list;
}

void refreshList(HWND hWnd, int IDC, std::list <std::string> list)
{
    HWND hList = GetDlgItem(hWnd, IDC);

    SendMessage(hList, LB_RESETCONTENT, (WPARAM)0, (LPARAM)0);

    for (auto i : list)
    {
        SendMessageA(hList, LB_ADDSTRING, 0, (LPARAM)i.c_str());
    }
}


void setTextToList(HWND hWnd, int IDC, std::list <std::string> list)
{
    HWND hList = GetDlgItem(hWnd, IDC);

    SendMessage(hList, LB_RESETCONTENT, (WPARAM)0, (LPARAM)0);

    for (auto i : list)
    {
        SendMessageA(hList, LB_ADDSTRING, 0, (LPARAM)i.c_str());
    }

}