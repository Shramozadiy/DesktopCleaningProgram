// DesktopManagement.cpp : Определяет точку входа для приложения.
//
#include "framework.h"
#include "DesktopManagement.h"

#define MAX_LOADSTRING 100


// Глобальные переменные:
HINSTANCE hInst;                                // текущий экземпляр
WCHAR szTitle[MAX_LOADSTRING];                  // Текст строки заголовка
WCHAR szWindowClass[MAX_LOADSTRING];            // имя класса главного окна

// Отправить объявления функций, включенных в этот модуль кода:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);




int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // Инициализация
    Data::getInstance().loadData("settings.ini");
    Desktop::getInstance().initDesktop();
    Data::getInstance().setFileAgeLimits(10);

    // Инициализация глобальных строк
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_DESKTOPMANAGEMENT, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Выполнить инициализацию приложения:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_DESKTOPMANAGEMENT));

    
    int start = clock();
    

    // Цикл основного сообщения:
    MSG msg;
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }

    }

    return (int) msg.wParam;
}



//
//  ФУНКЦИЯ: MyRegisterClass()
//
//  ЦЕЛЬ: Регистрирует класс окна.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_DESKTOPMANAGEMENT));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_DESKTOPMANAGEMENT);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   ФУНКЦИЯ: InitInstance(HINSTANCE, int)
//
//   ЦЕЛЬ: Сохраняет маркер экземпляра и создает главное окно
//
//   КОММЕНТАРИИ:
//
//        В этой функции маркер экземпляра сохраняется в глобальной переменной, а также
//        создается и выводится главное окно программы.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // Сохранить маркер экземпляра в глобальной переменной

   RECT rect = {0, 0, 800, 600};

   AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW & ~WS_THICKFRAME & ~WS_MAXIMIZEBOX, 1);

   int width = (rect.right - rect.left); // Ширина окна
   int height = (rect.bottom - rect.top); // Высота окна

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW & ~WS_THICKFRAME & ~WS_MAXIMIZEBOX,
      CW_USEDEFAULT, 0, width, height, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }
  
   AppendMenuA(GetMenu(hWnd), 0, IDM_RUN, "Run");
   AppendMenuA(GetMenu(hWnd), 0, IDM_REFRESH, "Refresh");
   AppendMenuA(GetMenu(hWnd), 0, IDM_SETTINGS, "Settings");

   // Создаем кнопку для выбора исключений
   HWND bt_quit = CreateWindow(L"button", L">>", WS_VISIBLE | WS_CHILD, 375, 80, 50, 50, hWnd, (HMENU)IDC_ADD_EXCEPTIONS, 0, 0);
   // Создаем кнопку для удаления исключений
   HWND bt_quit2 = CreateWindow(L"button", L"<<", WS_VISIBLE | WS_CHILD, 375, 150, 50, 50, hWnd, (HMENU)IDC_DELETE_EXCEPTIONS, 0, 0);
   // Создаем кнопку для выбора папки хранилища
   HWND bt_quit3 = CreateWindow(L"button", L">>", WS_VISIBLE | WS_CHILD, 375, 400, 50, 50, hWnd, (HMENU)IDC_SET_REPOSITORY, 0, 0);

   // Создаем поле для выбора исключений
   HWND hListToSelectExceptions = CreateWindow(L"listbox", L"", 
                                               WS_CHILD | WS_VISIBLE | WS_VSCROLL | LBS_DISABLENOSCROLL | LBS_NOTIFY | LBS_MULTIPLESEL | WS_BORDER, 
                                               10, 10, 355, 285, hWnd, (HMENU)IDC_CHOOSE_EXCEPTIONS, 0, 0);
   std::list <std::string> dirs = Desktop::getInstance().getFileNames();
   setTextToList(hWnd, IDC_CHOOSE_EXCEPTIONS, dirs);
   SendMessage(hListToSelectExceptions, WM_SETREDRAW, TRUE, 0L);

   // Создаем поле исключений
   HWND hListExceptions = CreateWindow(L"listbox", L"", 
                                       WS_CHILD | WS_VISIBLE | WS_VSCROLL | LBS_DISABLENOSCROLL | LBS_NOTIFY | LBS_MULTIPLESEL | WS_BORDER, 
                                       435, 10, 355, 285, hWnd, (HMENU)IDC_EXCEPTIONS, 0, 0);
   setTextToList(hWnd, IDC_EXCEPTIONS, Data::getInstance().getExceptions());
   SendMessage(hListExceptions, WM_SETREDRAW, TRUE, 0L);

   // Создаем поле для выбора папки хранилища
   HWND hListToSelectRepository = CreateWindow(L"listbox", L"", 
                                               WS_CHILD | WS_VISIBLE | WS_VSCROLL | LBS_DISABLENOSCROLL | LBS_NOTIFY | WS_BORDER, 
                                               10, 305, 355, 285, hWnd, (HMENU)IDC_CHOOSE_REPOSITORY, 0, 0);
   setTextToList(hWnd, IDC_CHOOSE_REPOSITORY, dirs);
   SendMessage(hListToSelectRepository, WM_SETREDRAW, TRUE, 0L);

   // Создаем поле папки хранилища
   HWND hListRepository = CreateWindow(L"listbox", L"", 
                                       WS_CHILD | WS_VISIBLE | WS_VSCROLL | LBS_DISABLENOSCROLL | LBS_NOTIFY | WS_BORDER, 
                                       435, 305, 355, 285, hWnd, (HMENU)IDC_REPOSITORY, 0, 0);
   SendMessageA(hListRepository, LB_ADDSTRING, 0, (LPARAM)Data::getInstance().getRepository().c_str());
   SendMessage(hListRepository, WM_SETREDRAW, TRUE, 0L);
    
   // Активируем окно
   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  ФУНКЦИЯ: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  ЦЕЛЬ: Обрабатывает сообщения в главном окне.
//
//  WM_COMMAND  - обработать меню приложения
//  WM_PAINT    - Отрисовка главного окна
//  WM_DESTROY  - отправить сообщение о выходе и вернуться
//
//


LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // Разобрать выбор в меню:
            switch (wmId)
            {
            case IDC_ADD_EXCEPTIONS:
            {
                std::list <std::string> list = getListText(hWnd, IDC_CHOOSE_EXCEPTIONS);
                for (auto i : list)
                {
                    Data::getInstance().addException(i);
                }

                refreshList(hWnd, IDC_EXCEPTIONS, Data::getInstance().getExceptions());

                Data::getInstance().saveData("Settings.ini");
            }break;
            case IDC_DELETE_EXCEPTIONS:
            {
                std::list <std::string> list = getListText(hWnd, IDC_EXCEPTIONS);
                for (auto i : list)
                {
                    Data::getInstance().deleteException(i);
                }

                refreshList(hWnd, IDC_EXCEPTIONS, Data::getInstance().getExceptions());

                Data::getInstance().saveData("Settings.ini");
            }break;
            case IDC_SET_REPOSITORY:
            {
                Data::getInstance().setRepository(getTextFromList(hWnd, IDC_CHOOSE_REPOSITORY));
                std::list <std::string> list = { Data::getInstance().getRepository() };
                refreshList(hWnd, IDC_REPOSITORY, list);

                Data::getInstance().saveData("Settings.ini");
            }break;
            case IDM_RUN:
            {
                moveFilesToRepository();
            }break;
            case IDM_REFRESH:
            {
                std::list <std::string> dirs = Desktop::getInstance().getFileNames();
                setTextToList(hWnd, IDC_CHOOSE_EXCEPTIONS, dirs);
                setTextToList(hWnd, IDC_CHOOSE_REPOSITORY, dirs);

            }break;
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
        
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            // TODO: Добавьте сюда любой код прорисовки, использующий HDC...
            EndPaint(hWnd, &ps);
        }
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// Обработчик сообщений для окна "О программе".
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}
