#include "../lib/gui.h"

enum {
    ID_Quit = wxID_EXIT,
    ID_About = wxID_ABOUT
};

wxBEGIN_EVENT_TABLE(MyFrame, wxFrame)
    EVT_MENU(ID_Quit, MyFrame::OnQuit)
    EVT_MENU(ID_About, MyFrame::OnAbout)
wxEND_EVENT_TABLE()

MyFrame::MyFrame(const wxString& title, const wxPoint& pos, const wxSize& size)
    : wxFrame(NULL, wxID_ANY, title, pos, size)
{
    wxMenu *menuFile = new wxMenu;
    menuFile->Append(ID_About, "&About...\tF1", "Show about dialog");
    menuFile->AppendSeparator();
    menuFile->Append(ID_Quit, "E&xit\tAlt-X", "Quit this program");

    wxMenuBar *menuBar = new wxMenuBar;
    menuBar->Append(menuFile, "&File");
    SetMenuBar(menuBar);

    CreateStatusBar();
    SetStatusText("Welcome to Hangman!");

    createTextInput();
}

void MyFrame::createTextInput()
{
    wxBoxSizer *sizer = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer *horizontalSizer = new wxBoxSizer(wxHORIZONTAL);
    horizontalSizer->Add(new wxTextCtrl(this, wxID_ANY, "", wxDefaultPosition, wxSize(100, 20)),
                        0, wxALL | wxALIGN_CENTER_VERTICAL, 5);
    sizer->Add(horizontalSizer, 0, wxALIGN_CENTER_HORIZONTAL | wxTOP, 5);
    sizer->AddStretchSpacer(1);
    SetSizer(sizer);
}

void MyFrame::OnQuit(wxCommandEvent& WXUNUSED(event))
{
    Close(true);
}

void MyFrame::OnAbout(wxCommandEvent& WXUNUSED(event))
{
    wxMessageBox("This is a small Hangman game using wxWidgets", "About Hangman",
                 wxOK | wxICON_INFORMATION, this);
}
