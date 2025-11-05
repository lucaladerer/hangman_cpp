#include "../lib/gui.h"

uint8_t Gui::m_wrongGuesses = 0;

wxBEGIN_EVENT_TABLE(Gui, wxFrame)
    EVT_MENU(ID_Quit, Gui::OnQuit)
    EVT_MENU(ID_About, Gui::OnAbout)
    EVT_BUTTON(ID_Submit, Gui::OnSubmit)
    EVT_BUTTON(ID_Guess, Gui::OnGuess)
wxEND_EVENT_TABLE()

Gui::Gui(const wxString& title, const wxPoint& pos, const wxSize& size)
    : wxFrame(NULL, wxID_ANY, title, pos, size)
    , m_inputField(nullptr)
    , m_submitButton(nullptr)
    // , m_wrongGuesses(0)
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
    // ToDo: Create a text input field and submit button for entering the word to guess
    wxBoxSizer *sizer = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer *horizontalSizerInput = new wxBoxSizer(wxHORIZONTAL);
    // wxBoxSizer *horizontalSizerGuess = new wxBoxSizer(wxHORIZONTAL);

    // Create input field with placeholder text
    m_inputField = new wxTextCtrl(this, wxID_ANY, "", 
                                 wxDefaultPosition, wxSize(120, 20),
                                 wxTE_PROCESS_ENTER);
    m_inputField->SetHint("Enter a word");

    // Submit button
    m_submitButton = new wxButton(this, ID_Submit, "Submit");
    
    // Add controls to horizontal sizer
    horizontalSizerInput->Add(m_inputField, 0, wxALL | wxALIGN_CENTER_VERTICAL, 5);
    horizontalSizerInput->Add(m_submitButton, 0, wxALL | wxALIGN_CENTER_VERTICAL, 5);

    // // button to enter letter
    // m_guessInputField = new wxTextCtrl(this, wxID_ANY, "", 
    //                              wxDefaultPosition, wxSize(40, 20),
    //                              wxTE_PROCESS_ENTER);
    // m_guessInputField->SetHint("Enter a letter");
    // m_guessButton = new wxButton(this, ID_Guess, "Guess");

    // horizontalSizerGuess->Add(m_guessInputField, 0, wxALL | wxALIGN_CENTER_VERTICAL, 5);
    // horizontalSizerGuess->Add(m_guessButton, 0, wxALL | wxALIGN_CENTER_VERTICAL, 5);

    // Add horizontal sizer to main vertical sizer
    sizer->Add(horizontalSizerInput, 0, wxALIGN_CENTER_HORIZONTAL | wxTOP, 5);
    // sizer->Add(horizontalSizerGuess, 0, wxALIGN_CENTER_HORIZONTAL | wxTOP, 5);
    sizer->AddStretchSpacer(1);

    SetSizer(sizer);

    // Bind enter key in text control to submit
    m_inputField->Bind(wxEVT_TEXT_ENTER, [this](wxCommandEvent& event) {
        wxCommandEvent submitEvent(wxEVT_COMMAND_BUTTON_CLICKED, ID_Submit);
        ProcessEvent(submitEvent);
    });

    // m_guessInputField->Bind(wxEVT_TEXT_ENTER, [this](wxCommandEvent& event) {
    //     wxCommandEvent guessEvent(wxEVT_COMMAND_BUTTON_CLICKED, ID_Guess);
    //     ProcessEvent(guessEvent);
    // });
}

void Gui::OnSubmit(wxCommandEvent& WXUNUSED(event)) // Event after clicking submit button or pressing enter
{
    if (m_inputField && m_submitButton) {
        m_currentInput = m_inputField->GetValue();
        
        if (!m_currentInput.IsEmpty() && (m_currentInput.Length() <= 15) && m_currentInput.IsWord()) {
            m_currentInput = m_currentInput.MakeUpper();
            SetStatusText(wxString::Format("Word set: %s", m_currentInput));
            
            // clear and disable input field and button
            m_inputField->Clear();
            m_inputField->Disable();
            m_submitButton->Disable();
            
            m_inputField->SetHint("Word has been set");
            // StartGame(); // Start the game after setting the word
        } else {
            SetStatusText("Please enter only letters (max 15)");
            return;
        }

        // create logic for placing letters
        SetupHangmanLines();

        // create guess controls
        SetupGuessControls();
    }
}

void Gui::SetupHangmanLines() {
    // create logic for placing letters
        // Setup horizontal lines for each letter
        wxSizer* mainSizer = GetSizer();
        int wordLength = m_currentInput.Length();
        wxBoxSizer* letterBoxSizer = new wxBoxSizer(wxHORIZONTAL);

        for (int i = 0; i < wordLength; i++) {
            wxPanel* linePanel = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxSize(20, 2));
            linePanel->SetBackgroundColour(*wxBLACK);
            
            // Add space between lines
            if (i > 0) {
            letterBoxSizer->AddSpacer(10);
            }
            
            // Store the panel for later reference when revealing letters
            // NOTE: don't add the same panel twice. We'll add it inside the
            // verticalSizer below (so remove the direct add here).
            m_letterPanels.push_back(linePanel);

            // Create a static text for the letter (initially empty) and style it
            wxStaticText* letterText = new wxStaticText(this, wxID_ANY, "",
                                                       wxDefaultPosition, wxSize(36, 36),
                                                       wxALIGN_CENTER);
            // Style: larger bold font and contrasting background to resemble a box
            wxFont font = letterText->GetFont();
            font.SetPointSize(14);
            font.SetWeight(wxFONTWEIGHT_BOLD);
            letterText->SetFont(font);
            letterText->SetBackgroundColour(*wxWHITE);
            letterText->SetForegroundColour(*wxBLACK);
            letterText->SetMinSize(wxSize(36, 36));
            m_letterControls.push_back(letterText);
            
            // Position letter above line
            wxBoxSizer* verticalSizer = new wxBoxSizer(wxVERTICAL);
            // Center the letter above the line and give a small gap
            verticalSizer->Add(letterText, 0, wxALIGN_CENTER | wxBOTTOM, 4);
            verticalSizer->Add(linePanel, 0, wxALIGN_CENTER);
            
            // Add the vertical sizer which contains the letter text and the
            // line panel. The panel is only added once (inside this vertical
            // sizer) to avoid the "already in a sizer" assert.
            letterBoxSizer->Add(verticalSizer);
        }

        mainSizer->Add(letterBoxSizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 20);
        mainSizer->Layout();
}

void Gui::SetupGuessControls() {
    wxSizer* mainSizer = GetSizer();
    wxBoxSizer *horizontalSizerGuess = new wxBoxSizer(wxHORIZONTAL);

    // Create a spacer before the guess controls
    mainSizer->AddSpacer(20);

    m_guessInputField = new wxTextCtrl(this, wxID_ANY, "", 
                                 wxDefaultPosition, wxSize(40, 20),
                                 wxTE_PROCESS_ENTER);
    m_guessInputField->SetHint("Enter a letter");
    m_guessButton = new wxButton(this, ID_Guess, "Guess");

    horizontalSizerGuess->Add(m_guessInputField, 0, wxALL | wxALIGN_CENTER_VERTICAL, 5);
    horizontalSizerGuess->Add(m_guessButton, 0, wxALL | wxALIGN_CENTER_VERTICAL, 5);

    // Add horizontal sizer with centering flags
    mainSizer->Add(horizontalSizerGuess, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 10);
    
    // Force layout update
    mainSizer->Layout();
    Layout();

    m_guessInputField->Bind(wxEVT_TEXT_ENTER, [this](wxCommandEvent& event) {
        wxCommandEvent guessEvent(wxEVT_COMMAND_BUTTON_CLICKED, ID_Guess);
        ProcessEvent(guessEvent);
    });
}

void Gui::OnGuess(wxCommandEvent& WXUNUSED(event)) {

    if (m_guessInputField) {
        wxString guess = m_guessInputField->GetValue();
        if (guess.Length() != 1 || !guess.IsWord()) {
            SetStatusText("Please enter a single letter");
            m_guessInputField->Clear();
            return;
        }

        m_currentLetter = guess.Upper()[0];
        m_guessInputField->Clear();

        if (std::find(m_guessedLettersVector.begin(), m_guessedLettersVector.end(), guess.Upper()[0]) != m_guessedLettersVector.end()) {
            SetStatusText(wxString::Format("Letter %c has already been guessed", guess[0]));
            m_guessInputField->Clear();
            return;
        }

        // printf("guess: %c\n", guess[0]);

        bool correctGuess = false;
        for (const auto& ch : m_currentInput) {
            if (ch == m_currentLetter) { // Correct guess
                correctGuess = true;
                SetStatusText(wxString::Format("Correct guess: %c", m_currentLetter));
                m_guessedLettersVector.push_back(m_currentLetter);
                // continue;
            }
        }
        if (!correctGuess) {
            SetStatusText(wxString::Format("Incorrect guess: %c. Current wrong guesses: %d", m_currentLetter, m_wrongGuesses + 1));
            m_wrongGuesses++;
        }

        if (m_guessedLettersVector.size() == m_currentInput.length()) {
            SetStatusText("Congratulations! You've guessed the word!");
            // Optionally disable further input
            m_guessInputField->Disable();
            m_guessButton->Disable();
        }
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
