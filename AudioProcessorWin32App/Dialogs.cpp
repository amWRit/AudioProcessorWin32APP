#include "Dialogs.h"
#include "Utils.h"
#include <string>

//IDD_REVERB_DIALOG 200
//IDC_DELAY_TIME 201
//IDC_DECAY_FACTOR 202

INT_PTR CALLBACK ReverbDialogProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam) {
    static ReverbParameters* reverbParams;

    switch (message) {
    case WM_INITDIALOG:
        reverbParams = reinterpret_cast<ReverbParameters*>(lParam);

        // Set default values for Delay Time and Decay Factor in the textboxes
        wchar_t buffer[256];

        // Set default delay time if it's not zero
        swprintf(buffer, 256, L"%.2f", reverbParams->delayTime);  // Format as string
        SetDlgItemText(hDlg, IDC_DELAY_TIME, buffer);

        // Set default decay factor if it's not zero
        swprintf(buffer, 256, L"%.2f", reverbParams->decayFactor);  // Format as string
        SetDlgItemText(hDlg, IDC_DECAY_FACTOR, buffer);

        return TRUE;

    case WM_COMMAND:
        switch (LOWORD(wParam)) {
        case IDOK: {
            wchar_t buffer[256];
            GetDlgItemText(hDlg, IDC_DELAY_TIME, buffer, 256); // Delay Time
            reverbParams->delayTime = _wtof(buffer);

            GetDlgItemText(hDlg, IDC_DECAY_FACTOR, buffer, 256); // Decay Factor
            reverbParams->decayFactor = _wtof(buffer);

            EndDialog(hDlg, IDOK);
            break;
        }
        case IDCANCEL:
            EndDialog(hDlg, IDCANCEL);
            break;
        }
        return TRUE;
    }
    return FALSE;
}

INT_PTR CALLBACK VolumeDialogProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam) {
    static VolumeParameters* volumeParams;

    switch (message) {
    case WM_INITDIALOG:
        volumeParams = reinterpret_cast<VolumeParameters*>(lParam);

        // Set default values for Volume Factor in the textboxes
        wchar_t buffer[256];

        // Set default volume factor if it's not zero
        swprintf(buffer, 256, L"%.2f", volumeParams->volumeFactor);  // Format as string
        SetDlgItemText(hDlg, IDC_VOLUME, buffer);

        return TRUE;

    case WM_COMMAND:
        switch (LOWORD(wParam)) {
        case IDOK: {
            wchar_t buffer[256];
            GetDlgItemText(hDlg, IDC_VOLUME, buffer, 256); // Volume Factor
            volumeParams->volumeFactor = _wtof(buffer);

            EndDialog(hDlg, IDOK);
            break;
        }
        case IDCANCEL:
            EndDialog(hDlg, IDCANCEL);
            break;
        }
        return TRUE;
    }
    return FALSE;
}

INT_PTR CALLBACK SpeedDialogProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam) {
    static SpeedParameters* speedParams;
    switch (message) {
    case WM_INITDIALOG:
        speedParams = reinterpret_cast<SpeedParameters*>(lParam);

        // Set default values for Speed Factor in the textboxes
        wchar_t buffer[256];

        // Set default Speed factor if it's not zero
        swprintf(buffer, 256, L"%.2f", speedParams->speedFactor);  // Format as string
        SetDlgItemText(hDlg, IDC_SPEED, buffer);
        return TRUE;

    case WM_COMMAND:
        switch (LOWORD(wParam)) {
        case IDOK: {
            wchar_t buffer[256];
            GetDlgItemText(hDlg, IDC_SPEED, buffer, 256); // Speed Factor
            speedParams->speedFactor = _wtof(buffer);

            EndDialog(hDlg, IDOK);
            break;
        }
        case IDCANCEL:
            EndDialog(hDlg, IDCANCEL);
            break;
        }
        return TRUE;
    }
    return FALSE;
}

INT_PTR CALLBACK ExtractAudioDialogProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam) {
    static ExtractParameters* extractParams = nullptr;

    switch (message) {
    case WM_INITDIALOG: {
        // Initialize the dialog and set default values
        extractParams = reinterpret_cast<ExtractParameters*>(lParam);  // Set the passed parameter

        // Populate the combo box with instrument types
        HWND hwndCombo = GetDlgItem(hDlg, IDC_COMBO_INSTRUMENT_TYPE);

        // Loop through all possible InstrumentType values
        for (const auto& pair : instrumentTypeToString) {
            // Convert the enum to a wide string (since SendMessage requires LPARAM to be a wide string)
            std::wstring instrumentName = Utils::stringToWstring(pair.second); // Convert string to wstring
            SendMessage(hwndCombo, CB_ADDSTRING, 0, (LPARAM)instrumentName.c_str());
        }

        SendMessage(hwndCombo, CB_SETCURSEL, 0, 0); // Set default selection to "Guitar"
        return (INT_PTR)TRUE;
    }

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK) {
            // Get the selected instrument type from the combo box
            HWND hwndCombo = GetDlgItem(hDlg, IDC_COMBO_INSTRUMENT_TYPE);
            int selectedIndex = (int)SendMessage(hwndCombo, CB_GETCURSEL, 0, 0);

            wchar_t instrumentType[50];
            SendMessage(hwndCombo, CB_GETLBTEXT, selectedIndex, (LPARAM)instrumentType);

            // Convert the selected string to std::string
            std::wstring instrumentStr(instrumentType);
            std::string selectedInstrumentStr = Utils::WideToString(instrumentStr.c_str());

            // Search the string in the instrumentTypeToString map
            InstrumentType selectedInstrument = InstrumentType::Unknown;  // Default to Unknown
            for (const auto& pair : instrumentTypeToString) {
                if (pair.second == selectedInstrumentStr) {
                    selectedInstrument = pair.first;
                    break;
                }
            }

            // Set the selected instrument type in the ExtractParameters struct
            extractParams->instrumentType = selectedInstrument;

            std::string debugMessage = "\n\ninst type: " + selectedInstrumentStr;
            OutputDebugStringA(debugMessage.c_str());

            // Close the dialog
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        else if (LOWORD(wParam) == IDCANCEL) {
            // If the cancel button is pressed, close the dialog
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }

    return (INT_PTR)FALSE;
}