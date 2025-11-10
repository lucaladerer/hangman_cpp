#include "../lib/gui.h"
#include <wx/stdpaths.h>
#include <wx/filename.h>


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
    , m_hangmanImage(nullptr)
{
    // init Image handlers
    wxInitAllImageHandlers();

    wxMenu *menuFile = new wxMenu;
    menuFile->Append(ID_About, "&About...\tF1", "Show about dialog");
    menuFile->AppendSeparator();
    menuFile->Append(ID_Quit, "E&xit\tAlt-X", "Quit this program");

    wxMenuBar *menuBar = new wxMenuBar;
    menuBar->Append(menuFile, "&File");
    SetMenuBar(menuBar);

    CreateStatusBar();
    SetStatusText("Welcome to Hangman!");
    
    // Set white background for the entire frame
    SetBackgroundColour(*wxWHITE);

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
            if (ch == m_currentLetter) {
                correctGuess = true;
                SetStatusText(wxString::Format("Correct guess: %c", m_currentLetter));
                m_guessedLettersVector.push_back(m_currentLetter);

                // Find all occurrences of the letter in the word and reveal them
                size_t pos = 0;
                while ((pos = m_currentInput.find(ch, pos)) != wxString::npos) {
                    m_letterControls[pos]->SetLabel(wxString::Format("%c", ch));
                    pos++;
                }
            }
        }
        if (!correctGuess) {
            SetStatusText(wxString::Format("Incorrect guess: %c. Current wrong guesses: %d", m_currentLetter, m_wrongGuesses + 1));
            m_wrongGuesses++;
            SetHangmanImage(m_wrongGuesses);
        }

        if (m_guessedLettersVector.size() == m_currentInput.length()) {
            SetStatusText("Congratulations! You've guessed the word!");
            // Optionally disable further input
            m_guessInputField->Disable();
            m_guessButton->Disable();
        }
    }
}

void Gui::SetHangmanImage(uint8_t wrongGuessesIndex) {
    // Debug log
    wxLogDebug("SetHangmanImage called — wrongGuessesIndex=%d", wrongGuessesIndex);


    // Update the hangman image based on the number of wrong guesses
    wxString exePath = wxStandardPaths::Get().GetExecutablePath();
    wxString exeDir = wxPathOnly(wxStandardPaths::Get().GetExecutablePath());
    wxString imagePath = wxFileName(exeDir + "/../hangman_cpp/images",
                                wxString::Format("hangman_%d.png", wrongGuessesIndex))
                                .GetFullPath();

    // DEBUG
    // wxMessageBox(imagePath, "Image path used");

    wxBitmap bmp;
    if (!bmp.LoadFile(imagePath, wxBITMAP_TYPE_PNG)) {
        wxMessageBox(wxString::Format("Failed to load image: %s", imagePath), "Error", wxICON_ERROR);
        return;
    }

    if (!bmp.IsOk()) {
        wxMessageBox("Bitmap invalid after loading!", "Error");
        return;
    }


    wxSizer* mainSizer = GetSizer();
    if (!mainSizer) {
        wxLogError("Main sizer is null in SetHangmanImage!");
        return;
    }
    if (!m_inputField) {
        wxLogError("Input field is null in SetHangmanImage!");
    }


    // Create the static bitmap control on first use
    if (!m_hangmanImage) {
        m_hangmanImage = new wxStaticBitmap(this, wxID_ANY, bmp, wxDefaultPosition, wxSize(128, 128));
    } else {
        m_hangmanImage->SetBitmap(bmp);
    }

    // Ensure the image control is inserted shortly after the input-field sizer
    // so it appears vertically between the input controls and the letter boxes.
    // We search for the sizer that contains the input field and insert the image
    // right after it (if not already added).
    bool inserted = false;
    wxSizerItemList& children = mainSizer->GetChildren();
    int index = 0;
    for (wxSizerItemList::compatibility_iterator it = children.GetFirst(); it; it = it->GetNext(), ++index) {
        wxSizerItem* item = it->GetData();
        wxSizer* childSizer = item->GetSizer();
        if (!childSizer) continue;

        // Check if this child sizer contains the input field
        bool hasInputField = false;
        for (wxSizerItemList::compatibility_iterator it2 = childSizer->GetChildren().GetFirst(); it2; it2 = it2->GetNext()) {
            wxSizerItem* inner = it2->GetData();
            if (inner && inner->GetWindow() == m_inputField) {
                hasInputField = true;
                break;
            }
        }

        if (hasInputField) {
            // If the image isn't already in the main sizer, insert it after this index.
            if (m_hangmanImage->GetContainingSizer() != mainSizer) {
                mainSizer->Insert(index + 1, m_hangmanImage, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 10);
            } else {
                // If already in the sizer, ensure it's roughly in the right place by doing a layout.
                mainSizer->Layout();
            }
            inserted = true;
            break;
        }
    }

    // Fallback: if we couldn't find the input sizer, just add the image if it's not added yet
    if (!inserted && m_hangmanImage->GetContainingSizer() != mainSizer) {
        mainSizer->Add(m_hangmanImage, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 10);
    }

    // Refresh layout to show the updated image
    mainSizer->Layout();
    Layout();
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
