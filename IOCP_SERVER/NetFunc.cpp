#include "pch.h"
#include "NetFunc.h"

wstring StringToWString(const string& str)
{
    size_t llSize = str.size() + 1; //널문자
    std::wstring wstr(llSize, 0);   //크기 미리 할당

    size_t llNumCvt = 0;
    mbstowcs_s(&llNumCvt, &wstr[0], llSize, str.c_str(), llSize - 1);

    while (TRUE)
    {
        if (wstr.back() == L'\0')
        {
            //널문자 만나기 까지
            wstr.pop_back();
            break;
        }
        wstr.pop_back();
    }

    return wstr;
}

string WstringToString(const wstring& _str)
{
    int iSize = WideCharToMultiByte(CP_UTF8, 0, _str.c_str(), -1, NULL, 0, NULL, NULL);
    string str(iSize, 0);
    WideCharToMultiByte(CP_UTF8, 0, _str.c_str(), -1, &str[0], iSize, NULL, NULL);
    return str;
}