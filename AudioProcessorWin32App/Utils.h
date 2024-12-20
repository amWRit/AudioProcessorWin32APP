#pragma once
#include <string>
#include <algorithm>
#include <locale>
#include <codecvt>

class Utils {
public:
    static std::string getFileExtension(const std::string& filePath) {
        size_t dotPosition = filePath.find_last_of('.');
        if (dotPosition == std::string::npos) {
            return ""; // No extension found
        }
        std::string extension = filePath.substr(dotPosition + 1);
        std::transform(extension.begin(), extension.end(), extension.begin(), ::tolower);
        return extension;
    }

    // deprecated in c++17
    //// Converts a wide string (wchar_t*) to a UTF-8 encoded std::string
    //static std::string WideToString(const wchar_t* wideString) {
    //    if (!wideString) {
    //        return "";
    //    }
    //    std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> converter;
    //    return converter.to_bytes(wideString);
    //}

    static std::string WideToString(const wchar_t* wideString) {
        if (!wideString) {
            return "";
        }

        // Get the length of the wide string
        int bufferSize = WideCharToMultiByte(CP_UTF8, 0, wideString, -1, NULL, 0, NULL, NULL);
        if (bufferSize == 0) {
            return "";  // Return an empty string if the conversion fails
        }

        // Allocate a buffer for the UTF-8 string
        std::string utf8String(bufferSize - 1, '\0');  // The bufferSize includes the null terminator, so subtract 1
        WideCharToMultiByte(CP_UTF8, 0, wideString, -1, &utf8String[0], bufferSize, NULL, NULL);

        return utf8String;
    }

    static std::wstring stringToWstring(const std::string& str) {
        int size_needed = MultiByteToWideChar(CP_UTF8, 0, &str[0], (int)str.size(), NULL, 0);
        std::wstring wstr(size_needed, 0);
        MultiByteToWideChar(CP_UTF8, 0, &str[0], (int)str.size(), &wstr[0], size_needed);
        return wstr;
    }
};
