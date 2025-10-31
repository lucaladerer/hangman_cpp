// Main entry point for the wxWidgets application
// #define wxUSE_DPI_AWARE_MANIFEST 2
// #include <wx/msw/wx.rc>
#include <wx/wx.h>
#include "../lib/gui.h"

class MyApp : public wxApp {
public:
    virtual bool OnInit() override;
};

wxIMPLEMENT_APP(MyApp);

bool MyApp::OnInit() {
    Gui* frame = new Gui("Hangman", wxDefaultPosition, wxSize(800,600));
    frame->Show(true);
    return true;
}