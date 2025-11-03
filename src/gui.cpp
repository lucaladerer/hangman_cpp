#include "../lib/gui.h"

wxBEGIN_EVENT_TABLE(Gui, wxFrame)
    EVT_MENU(ID_Quit, Gui::OnQuit)
    EVT_MENU(ID_About, Gui::OnAbout)
    EVT_BUTTON(ID_Submit, Gui::OnSubmit)
wxEND_EVENT_TABLE()

Gui::Gui(const wxString& title, const wxPoint& pos, const wxSize& size)
    : wxFrame(NULL, wxID_ANY, title, pos, size)
    , m_inputField(nullptr)
    , m_submitButton(nullptr)
    , m_wrongGuesses(0)
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

void Gui::createTextInput()
{
    wxBoxSizer *sizer = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer *horizontalSizer = new wxBoxSizer(wxHORIZONTAL);

    // Create input field with placeholder text
    m_inputField = new wxTextCtrl(this, wxID_ANY, "", 
                                 wxDefaultPosition, wxSize(120, 20),
                                 wxTE_PROCESS_ENTER);
    m_inputField->SetHint("Enter a word");

    // Submit button
    m_submitButton = new wxButton(this, ID_Submit, "Submit");
    
    // Add controls to horizontal sizer
    horizontalSizer->Add(m_inputField, 0, wxALL | wxALIGN_CENTER_VERTICAL, 5);
    horizontalSizer->Add(m_submitButton, 0, wxALL | wxALIGN_CENTER_VERTICAL, 5);

    // Add horizontal sizer to main vertical sizer
    sizer->Add(horizontalSizer, 0, wxALIGN_CENTER_HORIZONTAL | wxTOP, 5);
    sizer->AddStretchSpacer(1);

    SetSizer(sizer);

    // Bind enter key in text control to submit
    m_inputField->Bind(wxEVT_TEXT_ENTER, [this](wxCommandEvent& event) {
        wxCommandEvent submitEvent(wxEVT_COMMAND_BUTTON_CLICKED, ID_Submit);
        ProcessEvent(submitEvent);
    });
}

void Gui::OnSubmit(wxCommandEvent& WXUNUSED(event)) // Event after clicking submit button or pressing enter
{
    if (m_inputField && m_submitButton) {
        m_currentInput = m_inputField->GetValue();
        
        if (!m_currentInput.IsEmpty() && (m_currentInput.Length() <= 15) && m_currentInput.IsWord()) {
            SetStatusText(wxString::Format("Word set: %s", m_currentInput));
            
            // clear and disable input field and button
            m_inputField->Clear();
            m_inputField->Disable();
            m_submitButton->Disable();
            
            m_inputField->SetHint("Word has been set");
            StartGame(); // Start the game after setting the word
        } else {
            SetStatusText("Please enter only letters (max 15)");
        }
    }
}

void Gui::StartGame() {
    // TODO: Implement game start logic
    // Placeholder for starting the game logic
}

void Gui::LetterGuess(wxCommandEvent& WXUNUSED(event)) {
    bool correctGuess = false;
    for (const auto& ch : m_currentInput) {
        if (ch == m_currentLetter) { // Correct guess
            correctGuess = true;
            SetStatusText(wxString::Format("Correct guess: %c", m_currentLetter));
            continue;
        }
    }
    if (!correctGuess) {
        SetStatusText(wxString::Format("Incorrect guess: %c", m_currentLetter));
        m_wrongGuesses++;
    }
}

void Gui::OnQuit(wxCommandEvent& WXUNUSED(event))
{
    Close(true);
}

void Gui::OnAbout(wxCommandEvent& WXUNUSED(event))
{
    wxMessageBox("This is a small Hangman game using wxWidgets", "About Hangman",
                 wxOK | wxICON_INFORMATION, this);
}
