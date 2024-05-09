#include "Http.h"

Http::Http()
{

}

Http::~Http()
{
    
} 

float Http::getCurrency(char* from, char* to)
{
    float result = 0.0f;
    wchar_t* objectName = nullptr;

    hSession = WinHttpOpen(L"HTTP Example/1.0", WINHTTP_ACCESS_TYPE_DEFAULT_PROXY, WINHTTP_NO_PROXY_NAME, WINHTTP_NO_PROXY_BYPASS, 0);
    if (!hSession) {
        return result;
    }

    hConnect = WinHttpConnect(hSession, L"api.exchangerate-api.com", INTERNET_DEFAULT_HTTPS_PORT, 0);
    if (!hConnect) {
        WinHttpCloseHandle(hSession);
        return result;
    }


    if (strcmp(from, "USD") == 0)
        objectName = const_cast<wchar_t*>(L"/v4/latest/USD");
    else if (strcmp(from, "RUB") == 0)
        objectName = const_cast<wchar_t*>(L"/v4/latest/RUB");
    else if (strcmp(from, "EUR") == 0)
        objectName = const_cast<wchar_t*>(L"/v4/latest/EUR");
    else if (strcmp(from, "QAR") == 0)
        objectName = const_cast<wchar_t*>(L"/v4/latest/QAR");

    hRequest = WinHttpOpenRequest(hConnect, L"GET", objectName, NULL, WINHTTP_NO_REFERER, WINHTTP_DEFAULT_ACCEPT_TYPES, WINHTTP_FLAG_SECURE);
    if (!hRequest) {
        WinHttpCloseHandle(hConnect);
        WinHttpCloseHandle(hSession);
        return result;
    }

    if (!WinHttpSendRequest(hRequest, WINHTTP_NO_ADDITIONAL_HEADERS, 0, WINHTTP_NO_REQUEST_DATA, 0, 0, 0)) {
        WinHttpCloseHandle(hRequest);
        WinHttpCloseHandle(hConnect);
        WinHttpCloseHandle(hSession);
        return result;
    }

    if (!WinHttpReceiveResponse(hRequest, NULL)) {
        WinHttpCloseHandle(hRequest);
        WinHttpCloseHandle(hConnect);
        WinHttpCloseHandle(hSession);
        return result;
    }

    statusConnect = true;

    DWORD dwSize = 0;
    DWORD dwDownloaded = 0;
    char* pszOutBuffer = nullptr;

    do {
        dwSize += 2048;
        pszOutBuffer = new char[dwSize + 1];
        if (!pszOutBuffer) {
            WinHttpCloseHandle(hRequest);
            WinHttpCloseHandle(hConnect);
            WinHttpCloseHandle(hSession);
            return result;
        }

        ZeroMemory(pszOutBuffer, dwSize + 1);

        if (!WinHttpReadData(hRequest, pszOutBuffer, dwSize, &dwDownloaded)) {
            delete[] pszOutBuffer;
            WinHttpCloseHandle(hRequest);
            WinHttpCloseHandle(hConnect);
            WinHttpCloseHandle(hSession);
            return result;
        }

        std::string jsonResponse(pszOutBuffer);

        size_t rubIndex = jsonResponse.find(reinterpret_cast<char*>(to));
        if (rubIndex != std::string::npos) {
            size_t valueStartIndex = jsonResponse.find_first_of("0123456789", rubIndex);
            size_t valueEndIndex = jsonResponse.find_first_not_of("0123456789.", valueStartIndex);
            if (valueStartIndex != std::string::npos && valueEndIndex != std::string::npos) {
                std::string rubValueStr = jsonResponse.substr(valueStartIndex, valueEndIndex - valueStartIndex);
                result = std::stof(rubValueStr);
            }
        }
        else {
            delete[] pszOutBuffer;
            WinHttpCloseHandle(hRequest);
            WinHttpCloseHandle(hConnect);
            WinHttpCloseHandle(hSession);
            return result;
        }

        delete[] pszOutBuffer;
    } while (dwDownloaded > 0);

    WinHttpCloseHandle(hRequest);
    WinHttpCloseHandle(hConnect);
    WinHttpCloseHandle(hSession);

    return result;
}