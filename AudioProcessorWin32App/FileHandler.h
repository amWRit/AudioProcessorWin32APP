#ifndef FILEHANDLER_H
#define FILEHANDLER_H

#include <windows.h>
#include <string>

class FileHandler
{
private:
    wchar_t inputFilePath[260]; // To store the input file path
    wchar_t outputFilePath[260]; // To store the output file path

public:
    FileHandler(); // Constructor

    // Function to open a file dialog and store the selected file path
    void OpenFileDialog(HWND hWnd);

    const wchar_t* GetInputFilePath() const; // Getter for input file path
    void SetInputFilePath(const wchar_t* path); // Setter for output file path
    const wchar_t* GetOutputFilePath() const; // Getter for output file path
    void SetOutputFilePath(const wchar_t* customFileName); // Setter for output file path
};

#endif // FILEHANDLER_H
#pragma once
