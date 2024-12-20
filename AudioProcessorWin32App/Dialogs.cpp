#include "Dialogs.h"
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

