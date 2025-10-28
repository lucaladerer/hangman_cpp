#ifndef HANGMAN_GUI_H
#define HANGMAN_GUI_H

#include <wx/wx.h>

class MyFrame : public wxFrame {
public:
    MyFrame(const wxString& title, const wxPoint& pos, const wxSize& size);
private:
    void OnQuit(wxCommandEvent& event);
    void OnAbout(wxCommandEvent& event);
    void createTextInput();
    wxDECLARE_EVENT_TABLE();
};

#endif // HANGMAN_GUI_H
