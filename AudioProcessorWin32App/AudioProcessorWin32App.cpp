// AudioProcessorWin32App.cpp : Defines the entry point for the application.
//

#include "framework.h"
#include "AudioProcessorWin32App.h"
#include <commdlg.h>  // For file dialogs
#include "FileHandler.h"

#include "Settings.h"
#include "AudioProcessingManager.h"
#include "AudioFileHandlerFactory.h"
//#include "AudioProcessingStrategyFactory.h"
//#include "FFTProcessor.h"
//#include "/InstrumentFactory.h"
//#include "SignalProcessor.h"
#include "AudioFile.h"
#include "Utils.h"

#define MAX_LOADSTRING 100

#define ID_OPEN_FILE 1001
#define ID_REVERSE 1002
#define ID_SAVE_FILE 2001

// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name
const wchar_t* strategyType;

HWND hwndOpenFileButton;
HWND hwndReverseButton;
HWND hwndSaveFileButton;
HWND hwndInputFilePathBox;
HWND hwndFactorBox;
wchar_t g_selectedFilePath[260] = { 0 }; // Global variable

FileHandler fileHandler;
std::shared_ptr<AudioFileHandler> audioFileHandler = nullptr;
AudioProcessingManager audioProcessingManager;

// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

//void OpenFileDialog(HWND hWnd);
//void OnReverseClicked(HWND hWnd);
//void OnSaveFileClicked(HWND hWnd);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPWSTR    lpCmdLine,
    _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: Place code here.

    // Initialize global strings
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_AUDIOPROCESSORWIN32APP, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance(hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_AUDIOPROCESSORWIN32APP));

    MSG msg;

    // Main message loop:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int)msg.wParam;
}



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDC_AUDIOPROCESSORWIN32APP));
    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_AUDIOPROCESSORWIN32APP);
    wcex.lpszClassName = szWindowClass;
    wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
    hInst = hInstance; // Store instance handle in our global variable

    HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

    if (!hWnd)
    {
        return FALSE;
    }

    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);

    return TRUE;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE: Processes messages for the main window.
//
//  WM_COMMAND  - process the application menu
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_CREATE:
        // Create the text box
        hwndInputFilePathBox = CreateWindowW(L"EDIT", L"", WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL,
            150, 50, 700, 25, hWnd, (HMENU)2, hInst, NULL);  // ES_AUTOHSCROLL adds horizontal scrolling if needed
        

        // Create the Open file button
        hwndOpenFileButton = CreateWindowW(L"BUTTON", L"Open file",
            WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
            250, 100, 100, 40, hWnd, (HMENU)ID_OPEN_FILE, hInst, NULL);

        // Create the Reverse button
        hwndOpenFileButton = CreateWindowW(L"BUTTON", L"Reverse",
            WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
            450, 100, 100, 40, hWnd, (HMENU)ID_REVERSE, hInst, NULL);

        // Create the Save file button
        hwndSaveFileButton = CreateWindowW(L"BUTTON", L"Save file",
            WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
            650, 100, 100, 40, hWnd, (HMENU)ID_SAVE_FILE, hInst, NULL);
        EnableWindow(hwndSaveFileButton, FALSE);
        break;
    case WM_COMMAND:
    {
        int wmId = LOWORD(wParam);
        // Parse the menu selections:
        switch (wmId)
        {
        case ID_OPEN_FILE:  // Open File Button click event (ID 1)
        {
            // Open the file dialog
            /*static FileHandler fileHandler;*/
            fileHandler.OpenFileDialog(hWnd);
            SetWindowTextW(hwndInputFilePathBox, fileHandler.GetInputFilePath());

            // Use the utility function to convert wide string to std::string
            std::string inputFilePath = Utils::WideToString(fileHandler.GetInputFilePath());

            audioFileHandler = audioProcessingManager.loadAudioFile(inputFilePath);
            if (audioFileHandler) {
                //audioProcessingManager.setFileHandler(audioFileHandler);
                MessageBox(hWnd, L"Audio loaded successfully!", L"Success", MB_OK);
            }
            else {
                MessageBox(hWnd, L"Failed to load audio file!", L"Error", MB_OK | MB_ICONERROR);
            }

            break;
        }

        case ID_REVERSE: // Reverse Audio
        {
            //OnReverseClicked(hWnd);
             strategyType = L"audioReverse";
            // Call the reverseAudio method from AudioProcessingManager
            bool success = audioProcessingManager.reverseAudio();

            //const char* str = success ? "true" : "false";
            //OutputDebugStringA(str);

            if (success) {
                EnableWindow(hwndSaveFileButton, TRUE);
                MessageBox(hWnd, L"Audio reversed successfully!", L"Success", MB_OK);
            }
            else
            {
                MessageBox(hWnd, L"Failed to reverse audio file!", L"Error", MB_OK | MB_ICONERROR);
                break;
            }
            break;
        }

        case ID_SAVE_FILE: // Save File
        {
            //OnSaveFileClicked(hWnd);
            wchar_t filename[MAX_PATH];
            swprintf_s(filename, MAX_PATH, L"%ls_output.wav", strategyType);

            // Set the output file path in fileHandler
            fileHandler.SetOutputFilePath(filename);
            std::string outputFilePath = Utils::WideToString(fileHandler.GetOutputFilePath());

            std::string debugMessage = "\n\nOutput file path: " + outputFilePath;
            OutputDebugStringA(debugMessage.c_str());

            std::vector<double> timeDomainSignal = audioProcessingManager.getProcessedSignal();

            
            if (timeDomainSignal.empty())
            {
                MessageBox(hWnd, L"Empty signal.", L"Error", MB_OK | MB_ICONERROR);
                break;
            }

            // Save the processed audio
            bool success = audioFileHandler->saveAudio(outputFilePath, timeDomainSignal, audioFileHandler->getSampleRate());
            if (success) {
                OutputDebugStringA("Audio saved successfully.\n");

                std::wstring message = L"Audio saved successfully!\n";
                message += fileHandler.GetOutputFilePath();  // Append the output file path

                MessageBox(hWnd, message.c_str(), L"Success", MB_OK);
            }
            else {
                OutputDebugStringA("Failed to save audio.\n");
                MessageBox(hWnd, L"Failed to save audio file!", L"Error", MB_OK | MB_ICONERROR);
                break;
            }
            break;
        }
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
        // TODO: Add any drawing code that uses hdc here...
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

// Message handler for about box.
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

// Open file dialog boox
// Help: https://cplusplus.com/forum/windows/169960/
// // NOT USED - just for backup
//void OpenFileDialog(HWND hWnd)
//{
//    OPENFILENAMEW ofn;       // Common dialog box structure
//    wchar_t szFile[260];     // Buffer for file name
//
//    // Initialize OPENFILENAMEW structure
//    ZeroMemory(&szFile, sizeof(szFile));
//    ZeroMemory(&ofn, sizeof(ofn));
//    ofn.lStructSize = sizeof(ofn);
//    ofn.hwndOwner = hWnd;
//    ofn.lpstrFile = szFile;
//    ofn.nMaxFile = sizeof(szFile);
//    ofn.lpstrFilter = L"Wave Files (*.wav)\0*.wav\0AIFF Files (*.aiff)\0*.aiff\0All Supported Files (*.wav;*.aiff)\0*.wav;*.aiff\0";
//    ofn.nFilterIndex = 1;
//    ofn.lpstrFileTitle = NULL;
//    ofn.nMaxFileTitle = 0;
//    ofn.lpstrInitialDir = NULL;
//    ofn.lpstrTitle = L"Select a File";
//    ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
//
//    // Display the file dialog
//    if (GetOpenFileNameW(&ofn) == TRUE)
//    {
//        // File selected, show the path in the message box
//        MessageBoxW(hWnd, ofn.lpstrFile, L"Selected File", MB_OK);
//        // File selected; set its path in the edit control
//        SetWindowTextW(hwndFilePathBox, ofn.lpstrFile);
//    }
//    else
//    {
//        // Handle the cancellation or error
//        DWORD error = CommDlgExtendedError();
//        if (error == 0)
//        {
//            // User canceled the dialog
//            MessageBoxW(hWnd, L"File selection canceled.", L"Info", MB_OK | MB_ICONINFORMATION);
//        }
//        else
//        {
//            // An error occurred
//            wchar_t errorMessage[256];
//            swprintf_s(errorMessage, L"Error occurred. Code: 0x%08X", error);
//            MessageBoxW(hWnd, errorMessage, L"Error", MB_OK | MB_ICONERROR);
//        }
//    }
//
//}