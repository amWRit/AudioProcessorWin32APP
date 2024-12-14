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

    // Converts a wide string (wchar_t*) to a UTF-8 encoded std::string
    static std::string WideToString(const wchar_t* wideString) {
        if (!wideString) {
            return "";
        }
        std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> converter;
        return converter.to_bytes(wideString);
    }
};
