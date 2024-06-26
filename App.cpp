#include "App.h"
#include "Http.h"

App::App()
{
    hwnd = NULL;
    ZeroMemory(&wc, sizeof(WNDCLASS));

    Http http;
    usdToRub = http.getCurrency(const_cast<char*>("USD"), const_cast<char*>("RUB"));
    eurToRub = http.getCurrency(const_cast<char*>("EUR"), const_cast<char*>("RUB"));
    qarToRub = http.getCurrency(const_cast<char*>("QAR"), const_cast<char*>("RUB"));
    eurToUsd = http.getCurrency(const_cast<char*>("EUR"), const_cast<char*>("USD"));
    qarToUsd = http.getCurrency(const_cast<char*>("QAR"), const_cast<char*>("USD"));
    eurToQar = http.getCurrency(const_cast<char*>("EUR"), const_cast<char*>("QAR"));
    statusConnect = http.statusConnect;
}

App::~App()
{

}

int App::Run(HINSTANCE hInst, int nCmdShow)
{
    wc = { 0 };

    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hbrBackground = reinterpret_cast<HBRUSH>(COLOR_WINDOW);
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hIcon = LoadIcon(hInst, MAKEINTRESOURCE(IDI_ICON1));
    wc.hInstance = hInst;
    wc.lpszClassName = L"MainWindowClass";
    wc.lpszMenuName = nullptr;
    wc.style = CS_VREDRAW | CS_HREDRAW;
    wc.lpfnWndProc = App::ApplicationProc;

    if (!RegisterClass(&wc))
        return 1;

    hwnd = CreateWindow(
        L"MainWindowClass",
        L"Win32 App",
        WS_POPUP | WS_VISIBLE,
        100,
        100,
        395,
        300,
        NULL,
        NULL,
        hInst,
        this);

    if (!hwnd)
        return 1;

    HFONT hFont_ArialBlack = CreateFont(19, 0, 0, 0, FW_REGULAR, 0, 0, 0, ANSI_CHARSET, OUT_TT_PRECIS, CLIP_DEFAULT_PRECIS, DRAFT_QUALITY, VARIABLE_PITCH, L"Arial Black");

    SendMessage(this->hFirstComboBox, WM_SETFONT, (WPARAM)hFont_ArialBlack, MAKELPARAM(TRUE, 0));
    SendMessage(this->hSecondComboBox, WM_SETFONT, (WPARAM)hFont_ArialBlack, MAKELPARAM(TRUE, 0));

    UpdateWindow(hwnd);
    ShowWindow(hwnd, nCmdShow);

    MSG msg = { 0 };

    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    DeleteObject(hFont_ArialBlack);
    return static_cast<int>(msg.wParam);
}

// Window message procedure
LRESULT CALLBACK App::WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg)
    {
    case WM_NCHITTEST:
    {
        // To move the window
        LRESULT hit = DefWindowProc(hwnd, uMsg, wParam, lParam);
        if (hit == HTCLIENT) {
            hit = HTCAPTION;
        }
        return hit;
    }
    case WM_PAINT:
    {
        onWindowPaint();
    }
    return 0;
    case WM_DRAWITEM:
    {
        pdis = reinterpret_cast<LPDRAWITEMSTRUCT>(lParam);
        switch (pdis->CtlID)
        {
        case ID_CLOSE_BUTTON:
        {
            if (pdis->itemState & ODS_SELECTED)
                this->bCloseButtonPressed = true;
            else
                this->bCloseButtonPressed = false;
            this->DrawCloseButton(pdis->hDC);
        }
        return 0;
        case ID_COLLAPSE_BUTTON:
        {
            if (pdis->itemState & ODS_SELECTED)
                this->bCollapseButtonPressed = true;
            else
                this->bCollapseButtonPressed = false;
            this->DrawCollapseButton(pdis->hDC);
        }
        return 0;
        case ID_CONVERT_BUTTON:
        {
            if (pdis->itemState & ODS_SELECTED)
                this->bConvertButtonPressed = true;
            else
                this->bConvertButtonPressed = false;
            this->DrawConvertButton(pdis->hDC);
        }
        return 0;
        default:
            break;
        }
    }
    return 0;
    case WM_COMMAND:
    {
        switch (wParam)
        {
            
            case ID_CLOSE_BUTTON:
            {
                this->onCloseButtonClicked();
            }

            case ID_COLLAPSE_BUTTON:
            {
                this->onCollapseButtonClicked();
            }

            case ID_CONVERT_BUTTON:
            {
                this->onConvertButtonClicked();
            }
            return 0;
            default:
                return 0;
        }
    }
    return 0;
    case WM_CREATE:
    {
        this->CreateNativeControls();
        this->addItemsToFirstComboBox();
        this->addItemsToSecondComboBox();
    }
    return 0;
    case WM_DESTROY:
    {
        PostQuitMessage(0);
    }
    return 0;
    default:
        return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }
}

// Static application message procedure
LRESULT App::ApplicationProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    App* pApp;
    if (uMsg == WM_NCCREATE)
    {
        pApp = static_cast<App*>(reinterpret_cast<CREATESTRUCT*>(lParam)->lpCreateParams);
        SetLastError(0);
        if (!SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pApp)))
        {
            if (GetLastError() != 0)
                return false;
        }
    }
    else
    {
        pApp = reinterpret_cast<App*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
    }

    if (pApp)
    {
        pApp->hwnd = hwnd;
        return pApp->WndProc(hwnd, uMsg, wParam, lParam);
    }

    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

void App::onWindowPaint()
{
    PAINTSTRUCT ps;
    HDC hdc = BeginPaint(this->hwnd, &ps);

    HFONT hFont_ArialBlack = CreateFont(20, 0, 0, 0, FW_HEAVY, 0, 0, 0, ANSI_CHARSET, OUT_TT_PRECIS, CLIP_DEFAULT_PRECIS, DRAFT_QUALITY, VARIABLE_PITCH, L"Arial Black");
    HFONT hFont_ArialBlack_Title = CreateFont(25, 0, 0, 0, FW_HEAVY, 0, 0, 0, ANSI_CHARSET, OUT_TT_PRECIS, CLIP_DEFAULT_PRECIS, DRAFT_QUALITY, VARIABLE_PITCH, L"Arial Black");

    HBRUSH hWindowBrush = CreateSolidBrush(RGB(46, 46, 46));
    HBRUSH hTitleBrush = CreateSolidBrush(RGB(28, 28, 28));

    RECT rect;
    
    GetClientRect(this->hwnd, &rect);

    SetTextColor(hdc, RGB(255, 255, 255));
    SetBkColor(hdc, RGB(46, 46, 46));

    SelectObject(hdc, hWindowBrush);
    SelectObject(hdc, hFont_ArialBlack);
    Rectangle(hdc, rect.left, rect.top, rect.right, rect.bottom);

    TextOut(hdc, 20, 57, L"From", 4);
    TextOut(hdc, 20, 110, L"To", 4);

    SetTextColor(hdc, RGB(38, 38, 38));
    TextOut(hdc, 5, 275, L"C0D3D by: enihma", 18);

    if (statusConnect)
    {
        SetTextColor(hdc, RGB(2, 186, 51));
        TextOut(hdc, 250, 275, L"Status: SUCCESS", 16);
    }
    else
    {
        SetTextColor(hdc, RGB(181, 4, 4));
        TextOut(hdc, 250, 275, L"Status: ERROR", 16);
    }
    // Title
    SetTextColor(hdc, RGB(255, 255, 255));
    SetBkColor(hdc, RGB(28, 28, 28));

    SelectObject(hdc, hFont_ArialBlack_Title);
    SelectObject(hdc, hTitleBrush);
    Rectangle(hdc, rect.left, rect.top, rect.right, 35);
    TextOut(hdc, 8, 4, L"Converter", 9);

    DeleteObject(hWindowBrush);
    DeleteObject(hTitleBrush);
    DeleteObject(hFont_ArialBlack);
    DeleteObject(hFont_ArialBlack_Title);

    EndPaint(this->hwnd, &ps);
    
    HFONT hFont_Arial = CreateFont(27, 0, 0, 0, FW_REGULAR, 0, 0, 0, ANSI_CHARSET, OUT_TT_PRECIS, CLIP_DEFAULT_PRECIS, DRAFT_QUALITY, VARIABLE_PITCH, L"Arial");

    SendMessage(this->hFirstEdit, WM_SETFONT, (WPARAM)hFont_Arial, TRUE);
    SendMessage(this->hSecondEdit, WM_SETFONT, (WPARAM)hFont_Arial, TRUE);

    DeleteObject(hFont_Arial);
}


void App::CreateNativeControls()
{
    hCloseButton = CreateWindow(L"BUTTON", L"", WS_CHILD | WS_VISIBLE | BS_OWNERDRAW, 375, 7, 15, 15, hwnd, reinterpret_cast<HMENU>(this->ID_CLOSE_BUTTON), NULL, NULL);
    hCollapseButton = CreateWindow(L"BUTTON", L"", WS_CHILD | WS_VISIBLE | BS_OWNERDRAW, 355, 5, 15, 15, hwnd, reinterpret_cast<HMENU>(this->ID_COLLAPSE_BUTTON), NULL, NULL);
    hConvertButton = CreateWindow(L"BUTTON", L"", WS_CHILD | WS_VISIBLE | BS_OWNERDRAW, 100, 200, 220, 40, hwnd, reinterpret_cast<HMENU>(this->ID_CONVERT_BUTTON), NULL, NULL);

    hFirstEdit = CreateWindow(L"EDIT", L"", WS_CHILD | WS_VISIBLE, 100, 133, 220, 28, hwnd, NULL, NULL, NULL);
    hSecondEdit = CreateWindow(L"EDIT", L"", WS_CHILD | WS_VISIBLE, 100, 82, 220, 28, hwnd, NULL, NULL, NULL);

    hFirstComboBox = CreateWindow(L"COMBOBOX", L"", WS_CHILD | WS_VISIBLE | CBS_DROPDOWNLIST | BS_OWNERDRAW, 20, 82, 55, 20, hwnd, NULL, NULL, NULL);
    hSecondComboBox = CreateWindow(L"COMBOBOX", L"", WS_CHILD | WS_VISIBLE | CBS_DROPDOWNLIST | BS_OWNERDRAW, 20, 134, 55, 20, hwnd, NULL, NULL, NULL);
}

void App::DrawCloseButton(HDC hdc)
{
    RECT rc{ 0, 0, 25, 25 };
    HFONT hFont_ArialBlack = CreateFont(18, 0, 0, 0, FW_HEAVY, 0, 0, 0, ANSI_CHARSET, OUT_TT_PRECIS, CLIP_DEFAULT_PRECIS, DRAFT_QUALITY, VARIABLE_PITCH, L"Arial Black");
    HBRUSH bg;

    if (this->bCloseButtonPressed)
    {
        bg = CreateSolidBrush(RGB(28, 28, 28));

        FillRect(hdc, &rc, bg);

        SetBkColor(hdc, RGB(28, 28, 28));
        SetTextColor(hdc, RGB(200, 200, 200));
        
        SelectObject(hdc, hFont_ArialBlack);
        TextOut(hdc, 0, 0, L"x", 1);
        

        DeleteObject(bg);
        DeleteObject(hFont_ArialBlack);
    }
    else
    {
        bg = CreateSolidBrush(RGB(28, 28, 28));

        FillRect(hdc, &rc, bg);

        SetBkColor(hdc, RGB(28, 28, 28));
        SetTextColor(hdc, RGB(255, 255, 255));

        SelectObject(hdc, hFont_ArialBlack);
        TextOut(hdc, 0, 0, L"x", 1);

        DeleteObject(bg);
        DeleteObject(hFont_ArialBlack);
    }
}

void App::DrawCollapseButton(HDC hdc)
{
    RECT rc{ 0, 0, 25, 25 };
    HFONT hFont_ArialBlack = CreateFont(17, 0, 0, 0, FW_HEAVY, 0, 0, 0, ANSI_CHARSET, OUT_TT_PRECIS, CLIP_DEFAULT_PRECIS, DRAFT_QUALITY, VARIABLE_PITCH, L"Arial Black");
    HBRUSH bg;

    if (this->bCollapseButtonPressed)
    {
        bg = CreateSolidBrush(RGB(28, 28, 28));

        FillRect(hdc, &rc, bg);

        SetBkColor(hdc, RGB(28, 28, 28));
        SetTextColor(hdc, RGB(200, 200, 200));

        SelectObject(hdc, hFont_ArialBlack);
        TextOut(hdc, 0, 0, L"_", 1);


        DeleteObject(bg);
        DeleteObject(hFont_ArialBlack);

    }
    else
    {
        bg = CreateSolidBrush(RGB(28, 28, 28));

        FillRect(hdc, &rc, bg);

        SetBkColor(hdc, RGB(28, 28, 28));
        SetTextColor(hdc, RGB(255, 255, 255));

        SelectObject(hdc, hFont_ArialBlack);
        TextOut(hdc, 0, 0, L"_", 1);

        DeleteObject(bg);
        DeleteObject(hFont_ArialBlack);
    }
}

void App::DrawConvertButton(HDC hdc)
{
    RECT rc{ 0, 0, 220, 40 };
    HFONT hFont_Arial = CreateFont(30, 0, 0, 0, FW_REGULAR, 0, 0, 0, ANSI_CHARSET, OUT_TT_PRECIS, CLIP_DEFAULT_PRECIS, DRAFT_QUALITY, VARIABLE_PITCH, L"Arial Black");
    HBRUSH bg;

    if (this->bConvertButtonPressed)
    {
        bg = CreateSolidBrush(RGB(44, 97, 46));

        FillRect(hdc, &rc, bg);

        SetBkColor(hdc, RGB(44, 97, 46));
        SetTextColor(hdc, RGB(255, 255, 255));
        SelectObject(hdc, hFont_Arial);

        TextOut(hdc, 63, 3, L"Convert", 7);

        DeleteObject(bg);
        DeleteObject(hFont_Arial);
    }
    else
    {
        bg = CreateSolidBrush(RGB(67, 156, 70));

        FillRect(hdc, &rc, bg);

        SetBkColor(hdc, RGB(67, 156, 70));
        SetTextColor(hdc, RGB(255, 255, 255));
        SelectObject(hdc, hFont_Arial);

        TextOut(hdc, 63, 3, L"Convert", 7);

        DeleteObject(bg);
        DeleteObject(hFont_Arial);
    }
}

void App::onCloseButtonClicked()
{
    PostQuitMessage(0);
}

void App::onCollapseButtonClicked()
{
    ShowWindow(hwnd, SW_MINIMIZE);
}


void App::onConvertButtonClicked()
{
    if (this->bConvertButtonPressed)
    {
        wchar_t fbuff[256];
        wchar_t sbuff[256];

        wchar_t* getFirstComboBoxValue = nullptr;
        wchar_t* getSecondComboBoxValue = nullptr;
        this->getItemsFromComboBox(this->hFirstComboBox, &getFirstComboBoxValue);
        this->getItemsFromComboBox(this->hSecondComboBox, &getSecondComboBoxValue);

        GetWindowText(this->hFirstEdit, fbuff, 256);

        GetWindowText(this->hSecondEdit, sbuff, 256);

        double num1 = _wtof(fbuff);
        double num2 = _wtof(sbuff);

        double result = 0;
        
        if (wcscmp(getFirstComboBoxValue, L"RUB") == 0 && wcscmp(getSecondComboBoxValue, L"USD") == 0)
            result = num2 / this->usdToRub;
        if (wcscmp(getFirstComboBoxValue, L"USD") == 0 && wcscmp(getSecondComboBoxValue, L"RUB") == 0)
            result = num2 * this->usdToRub;

        if (wcscmp(getFirstComboBoxValue, L"RUB") == 0 && wcscmp(getSecondComboBoxValue, L"EUR") == 0)
            result = num2 / this->eurToRub;
        if (wcscmp(getFirstComboBoxValue, L"EUR") == 0 && wcscmp(getSecondComboBoxValue, L"RUB") == 0)
            result = num2 * this->eurToRub;

        if (wcscmp(getFirstComboBoxValue, L"RUB") == 0 && wcscmp(getSecondComboBoxValue, L"QAR") == 0)
            result = num2 / this->qarToRub;
        if (wcscmp(getFirstComboBoxValue, L"QAR") == 0 && wcscmp(getSecondComboBoxValue, L"RUB") == 0)
            result = num2 * this->qarToRub;

        if (wcscmp(getFirstComboBoxValue, L"USD") == 0 && wcscmp(getSecondComboBoxValue, L"EUR") == 0)
            result = num2 / this->eurToUsd;
        if (wcscmp(getFirstComboBoxValue, L"EUR") == 0 && wcscmp(getSecondComboBoxValue, L"USD") == 0)
            result = num2 * this->eurToUsd;

        if (wcscmp(getFirstComboBoxValue, L"USD") == 0 && wcscmp(getSecondComboBoxValue, L"QAR") == 0)
            result = num2 / this->qarToUsd;
        if (wcscmp(getFirstComboBoxValue, L"QAR") == 0 && wcscmp(getSecondComboBoxValue, L"USD") == 0)
            result = num2 * this->qarToUsd;

        if (wcscmp(getFirstComboBoxValue, L"QAR") == 0 && wcscmp(getSecondComboBoxValue, L"EUR") == 0)
            result = num2 / this->eurToQar;
        if (wcscmp(getFirstComboBoxValue, L"EUR") == 0 && wcscmp(getSecondComboBoxValue, L"QAR") == 0)
            result = num2 * this->eurToQar;

        std::wstring resultString;

        if (result == 0.0f)
            resultString = L"invalid data!";
        else
            resultString = std::to_wstring(result);

        SetWindowText(this->hFirstEdit, resultString.c_str());

        delete[] getFirstComboBoxValue;
        delete[] getSecondComboBoxValue;
    }
}

void App::addItemsToFirstComboBox()
{
    SendMessage(this->hFirstComboBox, CB_ADDSTRING, 0, (LPARAM)L"RUB");
    SendMessage(this->hFirstComboBox, CB_ADDSTRING, 0, (LPARAM)L"USD");
    SendMessage(this->hFirstComboBox, CB_ADDSTRING, 0, (LPARAM)L"QAR");
    SendMessage(this->hFirstComboBox, CB_ADDSTRING, 0, (LPARAM)L"EUR");

    SendMessage(this->hFirstComboBox, CB_SETCURSEL, 0, 0);
}

void App::addItemsToSecondComboBox()
{
    SendMessage(this->hSecondComboBox, CB_ADDSTRING, 0, (LPARAM)L"RUB");
    int indexUSD = SendMessage(this->hSecondComboBox, CB_ADDSTRING, 0, (LPARAM)L"USD");
    SendMessage(this->hSecondComboBox, CB_ADDSTRING, 0, (LPARAM)L"QAR");
    SendMessage(this->hSecondComboBox, CB_ADDSTRING, 0, (LPARAM)L"EUR");

    SendMessage(this->hSecondComboBox, CB_SETCURSEL, indexUSD, 0);
}

void App::getItemsFromComboBox(HWND hComboBox, wchar_t** buffer)
{
    int selectIndex = SendMessage(hComboBox , CB_GETCURSEL, 0, 0);

    int textLength = SendMessage(hComboBox, CB_GETLBTEXTLEN, selectIndex, 0);

    *buffer = new wchar_t[textLength + 1];

    SendMessage(hComboBox, CB_GETLBTEXT, selectIndex, (LPARAM)(*buffer));
}