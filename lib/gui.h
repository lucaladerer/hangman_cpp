#ifndef HANGMAN_GUI_H
#define HANGMAN_GUI_H

#include <wx/wx.h>
#include <vector>

// Define custom event IDs
enum {
    ID_Quit = wxID_EXIT,
    ID_About = wxID_ABOUT,
    ID_Submit = wxID_HIGHEST + 1,   
    ID_Guess = wxID_HIGHEST + 2
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
    void OnGuess(wxCommandEvent& event);
    void SetupGuessControls();
    void SetupHangmanLines();
    void createTextInput();
    void SetHangmanImage(uint8_t wrongGuessesIndex);
    wxString ConcatWrongGuessesString();

    // UI Controls
    wxTextCtrl* m_inputField;
    wxTextCtrl* m_guessInputField;
    wxButton* m_submitButton;
    wxButton* m_guessButton;

    // Game state
    wxString m_currentInput;
    wxUniChar m_currentLetter;

    std::vector<wxPanel*> m_letterPanels;
    std::vector<wxStaticText*> m_letterControls;
    std::vector<wxUniChar> m_guessedLettersVector;
    std::vector<wxUniChar> m_wrongGuessedLettersVector;

    wxStaticBitmap* m_hangmanImage;

    static uint8_t m_wrongGuesses;

    wxDECLARE_EVENT_TABLE();
};

#endif // HANGMAN_GUI_H
