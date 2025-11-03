#ifndef HANGMAN_GUI_H
#define HANGMAN_GUI_H

#include <wx/wx.h>

// Define custom event IDs
enum {
    ID_Quit = wxID_EXIT,
    ID_About = wxID_ABOUT,
    ID_Submit = wxID_HIGHEST + 1
};

class Gui : public wxFrame {
public:
    Gui(const wxString& title, const wxPoint& pos, const wxSize& size);
    
    // Get the current input text
    wxString GetCurrentInput() const { return m_currentInput; }

private:
    void OnQuit(wxCommandEvent& event);
    void OnAbout(wxCommandEvent& event);
    void OnSubmit(wxCommandEvent& event);
    void LetterGuess(wxCommandEvent& event);
    void createTextInput();

    // UI Controls
    wxTextCtrl* m_inputField;
    wxButton* m_submitButton;
    
    // Game state
    wxString m_currentInput;
    wxUnichar m_currentLetter;

    static uint8_t m_wrongGuesses;

    wxDECLARE_EVENT_TABLE();
};

#endif // HANGMAN_GUI_H
