#pragma once
#ifndef DIALOGS_H
#define DIALOGS_H

#include <windows.h>
#include "resource.h"

// Function declarations for each dialog box
INT_PTR CALLBACK ReverbDialogProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
INT_PTR CALLBACK VolumeDialogProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
INT_PTR CALLBACK SpeedDialogProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);

// Additional helper functions, if needed
struct ReverbParameters {
    double delayTime;
    double decayFactor;

    // Constructor to initialize with default values
    ReverbParameters() : delayTime(1.0), decayFactor(0.5) {}  // Default values
};

struct VolumeParameters {
    double volumeFactor;
    // Constructor to initialize with default values
    VolumeParameters() : volumeFactor(0.5){}  // Default values
};

struct SpeedParameters {
    double speedFactor;
    // Constructor to initialize with default values
    SpeedParameters() : speedFactor(2.0) {}  // Default values
};

#endif // DIALOGS_H
