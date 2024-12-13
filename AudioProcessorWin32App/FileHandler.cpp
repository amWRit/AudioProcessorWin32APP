#include "FileHandler.h"
#include <commdlg.h>
#include <string>

#include <windows.h>
#include <shlobj.h>
#include <iostream>

FileHandler::FileHandler()
{
    // Initialize selectedFilePath with an empty string
    ZeroMemory(&inputFilePath, sizeof(inputFilePath));
    ZeroMemory(&outputFilePath, sizeof(outputFilePath));
}

const wchar_t* FileHandler::GetInputFilePath() const
{
    return inputFilePath;
}

const wchar_t* FileHandler::GetOutputFilePath() const
{
    return outputFilePath;
}

void FileHandler::OpenFileDialog(HWND hWnd)
{
    OPENFILENAMEW ofn;
    ZeroMemory(&ofn, sizeof(ofn));
    wchar_t szFile[260] = { 0 }; // Local buffer for file dialog
    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = hWnd;
    ofn.lpstrFile = szFile;
    ofn.nMaxFile = sizeof(szFile) / sizeof(szFile[0]);
    ofn.lpstrFilter = L"WAV and AIFF Files (*.wav;*.aiff)\0*.wav;*.aiff\0All Files (*.*)\0*.*\0";
    ofn.nFilterIndex = 1;
    ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

    if (GetOpenFileNameW(&ofn) == TRUE)
    {
        // Store the selected file path in the class member
        FileHandler::SetInputFilePath(ofn.lpstrFile);

        //testing ooutputfilepath
        //FileHandler::SetOutputFilePath(L"my_audio_file.wav");
    }
}

void FileHandler::SetInputFilePath(const wchar_t* path)
{
    // Copy the provided path into outputFilePath
    wcsncpy_s(inputFilePath, path, sizeof(inputFilePath) / sizeof(inputFilePath[0]));
}


void FileHandler::SetOutputFilePath(const wchar_t* customFileName)
{
    // Get the Documents folder path
    wchar_t documentsPath[MAX_PATH];
    HRESULT hr = SHGetFolderPathW(NULL, CSIDL_MYDOCUMENTS, NULL, 0, documentsPath);
    if (SUCCEEDED(hr))
    {
        // Debug: Output the Documents path to check if it's correct
        char str[256];
        sprintf_s(str, sizeof(str), "Documents Directory: %ws\n", documentsPath);
        OutputDebugStringA(str);

        // Define project name and output folder
        const wchar_t* projectName = L"AudioProcessor";  // Replace with actual project name
        const wchar_t* outputFolder = L"output";

        // Construct the full output file path: Documents folder + project_name + output + custom file name
        wchar_t fullFilePath[MAX_PATH];
        swprintf_s(fullFilePath, MAX_PATH, L"%s\\%s\\%s\\%s", documentsPath, projectName, outputFolder, customFileName);

        // Copy the result into the output file path
        wcsncpy_s(outputFilePath, fullFilePath, sizeof(outputFilePath) / sizeof(outputFilePath[0]));

        // Debug: Output the full output file path to check if it's correct
        sprintf_s(str, sizeof(str), "Output file path set to: %ws\n", outputFilePath);
        OutputDebugStringA(str);
    }
    else
    {
        // If SHGetFolderPathW fails, print an error message
        OutputDebugStringA("Failed to get Documents folder path.\n");
    }
}
