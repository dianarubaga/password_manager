// main.cpp
#include <wx/wx.h>
#include "ui.h"

int main(int argc, char** argv) {
    // Initialize wxWidgets
    wxInitializer initializer;
    if (!initializer) {
        std::cerr << "Failed to initialize wxWidgets." << std::endl;
        return -1;
    }

    // Start the wxWidgets event loop
    return wxEntry(argc, argv);
}
