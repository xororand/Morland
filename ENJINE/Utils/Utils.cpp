#include "Utils.h"
#include "intrin.h"
#include "Windows.h"
#include "stringapiset.h"
#include <codecvt>


std::string Utils::encoding::to_utf8(const std::wstring& wstr)
{
    if (wstr.empty()) return std::string();
    int size_needed = WideCharToMultiByte(CP_UTF8, 0, &wstr[0], (int)wstr.size(), NULL, 0, NULL, NULL);
    std::string strTo(size_needed, 0);
    WideCharToMultiByte(CP_UTF8, 0, &wstr[0], (int)wstr.size(), &strTo[0], size_needed, NULL, NULL);
    return strTo;
}
std::wstring Utils::encoding::to_wide(const std::string& str)
{
    if (str.empty()) return std::wstring();
    int size_needed = MultiByteToWideChar(CP_UTF8, 0, &str[0], (int)str.size(), NULL, 0);
    std::wstring wstrTo(size_needed, 0);
    MultiByteToWideChar(CP_UTF8, 0, &str[0], (int)str.size(), &wstrTo[0], size_needed);
    return wstrTo;
}

std::string Utils::string::replace(std::string s,
    char c1, char c2)
{
    int l = s.length();

    // loop to traverse in the string
    for (int i = 0; i < l; i++)
    {
        // check for c1 and replace
        if (s[i] == c1)
            s[i] = c2;

        // check for c2 and replace
        else if (s[i] == c2)
            s[i] = c1;
    }
    return s;
}
std::wstring Utils::string::replace(std::wstring s,
    char c1, char c2)
{
    int l = s.length();

    // loop to traverse in the string
    for (int i = 0; i < l; i++)
    {
        // check for c1 and replace
        if (s[i] == c1)
            s[i] = c2;
    }
    return s;
}