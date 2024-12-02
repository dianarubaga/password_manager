// ui.cpp
#include "ui.h"
#include <wx/msgdlg.h>
#include <wx/textctrl.h>
#include <wx/valtext.h>
#include <wx/valnum.h>
#include <sstream>
#include <fstream>
#include <iomanip>
#include "encryption.h"

namespace PasswordNS {

// Implement the Application Class
wxIMPLEMENT_APP(PasswordManagerApp);

bool PasswordManagerApp::OnInit() {
    PasswordManager* manager = new PasswordManager();
    LoginFrame* login = new LoginFrame("Password Manager - Login/Register", *manager);
    login->Show(true);
    return true;
}

// Implement the Login/Register Frame
LoginFrame::LoginFrame(const wxString& title, PasswordManager& manager)
    : wxFrame(NULL, wxID_ANY, title, wxDefaultPosition, wxSize(400, 300)),
      passwordManager(manager) {

    wxPanel* panel = new wxPanel(this, wxID_ANY);

    wxBoxSizer* vbox = new wxBoxSizer(wxVERTICAL);

    // Username
    wxBoxSizer* hbox1 = new wxBoxSizer(wxHORIZONTAL);
    wxStaticText* userLabel = new wxStaticText(panel, wxID_ANY, wxT("Username:"));
    hbox1->Add(userLabel, 0, wxRIGHT, 8);
    usernameCtrl = new wxTextCtrl(panel, wxID_ANY);
    hbox1->Add(usernameCtrl, 1);
    vbox->Add(hbox1, 0, wxEXPAND | wxLEFT | wxRIGHT | wxTOP, 10);

    // Password
    wxBoxSizer* hbox2 = new wxBoxSizer(wxHORIZONTAL);
    wxStaticText* passLabel = new wxStaticText(panel, wxID_ANY, wxT("Password:"));
    hbox2->Add(passLabel, 0, wxRIGHT, 8);
    passwordCtrl = new wxTextCtrl(panel, wxID_ANY, "", wxDefaultPosition, wxDefaultSize, wxTE_PASSWORD);
    hbox2->Add(passwordCtrl, 1);
    vbox->Add(hbox2, 0, wxEXPAND | wxLEFT | wxRIGHT | wxTOP, 10);

    // Message
    message = new wxStaticText(panel, wxID_ANY, wxT(""), wxDefaultPosition, wxDefaultSize, wxST_NO_AUTORESIZE);
    vbox->Add(message, 0, wxALIGN_CENTER | wxTOP, 10);

    // Buttons
    wxBoxSizer* hbox3 = new wxBoxSizer(wxHORIZONTAL);
    loginButton = new wxButton(panel, wxID_ANY, wxT("Login"));
    registerButton = new wxButton(panel, wxID_ANY, wxT("Register"));
    hbox3->Add(loginButton, 1);
    hbox3->Add(registerButton, 1, wxLEFT, 5);
    vbox->Add(hbox3, 0, wxALIGN_CENTER | wxTOP | wxBOTTOM, 10);

    panel->SetSizer(vbox);

    // Bind Events
    Bind(wxEVT_BUTTON, &LoginFrame::OnLogin, this, loginButton->GetId());
    Bind(wxEVT_BUTTON, &LoginFrame::OnRegister, this, registerButton->GetId());
}

void LoginFrame::OnLogin(wxCommandEvent& event) {
    wxString username = usernameCtrl->GetValue();
    wxString password = passwordCtrl->GetValue();

    if (username.IsEmpty() || password.IsEmpty()) {
        message->SetLabel("Please enter both username and password.");
        return;
    }

    passwordManager.setTestCredentials(std::string(username.mb_str()), std::string(password.mb_str()));

    try {
        if (passwordManager.loadUserCredentialsFromFile()) {
            // Successful login
            wxMessageBox("Login successful!", "Info", wxOK | wxICON_INFORMATION);
            MainMenuFrame* mainMenu = new MainMenuFrame("Password Manager - Main Menu", passwordManager);
            mainMenu->Show(true);
            this->Close();
        } else {
            wxMessageBox("Invalid username or password.", "Error", wxOK | wxICON_ERROR);
        }
    }
    catch (const std::exception& e) {
        wxMessageBox(e.what(), "Error", wxOK | wxICON_ERROR);
    }
}

void LoginFrame::OnRegister(wxCommandEvent& event) {
    wxString username = usernameCtrl->GetValue();
    wxString password = passwordCtrl->GetValue();

    if (username.IsEmpty() || password.IsEmpty()) {
        message->SetLabel("Please enter both username and password.");
        return;
    }

    passwordManager.setTestCredentials(std::string(username.mb_str()), std::string(password.mb_str()));

    try {
        if (!passwordManager.validate(std::string(password.mb_str()))) {
            wxMessageBox("Password is too weak. Must be longer than 8 characters.", "Error", wxOK | wxICON_ERROR);
            return;
        }

        passwordManager.saveUserCredentialsToFile();
        wxMessageBox("Registration successful! You can now log in.", "Info", wxOK | wxICON_INFORMATION);
    }
    catch (const std::exception& e) {
        wxMessageBox(e.what(), "Error", wxOK | wxICON_ERROR);
    }
}

// Implement the Main Menu Frame
MainMenuFrame::MainMenuFrame(const wxString& title, PasswordManager& manager)
    : wxFrame(NULL, wxID_ANY, title, wxDefaultPosition, wxSize(500, 400)),
      passwordManager(manager) {

    wxPanel* panel = new wxPanel(this, wxID_ANY);

    wxBoxSizer* vbox = new wxBoxSizer(wxVERTICAL);

    // Welcome Message
    std::string welcome = "Welcome, " + passwordManager.getUsername();
    wxStaticText* welcomeText = new wxStaticText(panel, wxID_ANY, wxString(welcome));
    vbox->Add(welcomeText, 0, wxALIGN_CENTER | wxTOP, 10);

    // Buttons
    addPasswordButton = new wxButton(panel, wxID_ANY, wxT("Add a New Password"));
    generatePasswordButton = new wxButton(panel, wxID_ANY, wxT("Generate a Password"));
    retrievePasswordButton = new wxButton(panel, wxID_ANY, wxT("Retrieve a Password"));
    deletePasswordButton = new wxButton(panel, wxID_ANY, wxT("Delete a Password"));
    viewAllPasswordsButton = new wxButton(panel, wxID_ANY, wxT("View All Passwords"));
    exitButton = new wxButton(panel, wxID_ANY, wxT("Exit"));

    vbox->Add(addPasswordButton, 0, wxEXPAND | wxLEFT | wxRIGHT | wxTOP, 10);
    vbox->Add(generatePasswordButton, 0, wxEXPAND | wxLEFT | wxRIGHT | wxTOP, 10);
    vbox->Add(retrievePasswordButton, 0, wxEXPAND | wxLEFT | wxRIGHT | wxTOP, 10);
    vbox->Add(deletePasswordButton, 0, wxEXPAND | wxLEFT | wxRIGHT | wxTOP, 10);
    vbox->Add(viewAllPasswordsButton, 0, wxEXPAND | wxLEFT | wxRIGHT | wxTOP, 10);
    vbox->Add(exitButton, 0, wxEXPAND | wxLEFT | wxRIGHT | wxTOP, 10);

    // Message
    message = new wxStaticText(panel, wxID_ANY, wxT(""), wxDefaultPosition, wxDefaultSize, wxST_NO_AUTORESIZE);
    vbox->Add(message, 0, wxALIGN_CENTER | wxTOP, 10);

    panel->SetSizer(vbox);

    // Bind Events
    Bind(wxEVT_BUTTON, &MainMenuFrame::OnAddPassword, this, addPasswordButton->GetId());
    Bind(wxEVT_BUTTON, &MainMenuFrame::OnGeneratePassword, this, generatePasswordButton->GetId());
    Bind(wxEVT_BUTTON, &MainMenuFrame::OnRetrievePassword, this, retrievePasswordButton->GetId());
    Bind(wxEVT_BUTTON, &MainMenuFrame::OnDeletePassword, this, deletePasswordButton->GetId());
    Bind(wxEVT_BUTTON, &MainMenuFrame::OnViewAllPasswords, this, viewAllPasswordsButton->GetId());
    Bind(wxEVT_BUTTON, &MainMenuFrame::OnExit, this, exitButton->GetId());
}

void MainMenuFrame::OnAddPassword(wxCommandEvent& event) {
    wxDialog* dialog = new wxDialog(this, wxID_ANY, "Add a New Password", wxDefaultPosition, wxSize(400, 300));
    wxPanel* panel = new wxPanel(dialog, wxID_ANY);
    wxBoxSizer* vbox = new wxBoxSizer(wxVERTICAL);

    // Service Name
    wxBoxSizer* hbox1 = new wxBoxSizer(wxHORIZONTAL);
    wxStaticText* serviceLabel = new wxStaticText(panel, wxID_ANY, wxT("Service Name:"));
    hbox1->Add(serviceLabel, 0, wxRIGHT, 8);
    wxTextCtrl* serviceCtrl = new wxTextCtrl(panel, wxID_ANY);
    hbox1->Add(serviceCtrl, 1);
    vbox->Add(hbox1, 0, wxEXPAND | wxLEFT | wxRIGHT | wxTOP, 10);

    // Service Username
    wxBoxSizer* hbox2 = new wxBoxSizer(wxHORIZONTAL);
    wxStaticText* userLabel = new wxStaticText(panel, wxID_ANY, wxT("Service Username:"));
    hbox2->Add(userLabel, 0, wxRIGHT, 8);
    wxTextCtrl* userCtrl = new wxTextCtrl(panel, wxID_ANY);
    hbox2->Add(userCtrl, 1);
    vbox->Add(hbox2, 0, wxEXPAND | wxLEFT | wxRIGHT | wxTOP, 10);

    // Password
    wxBoxSizer* hbox3 = new wxBoxSizer(wxHORIZONTAL);
    wxStaticText* passLabel = new wxStaticText(panel, wxID_ANY, wxT("Password:"));
    hbox3->Add(passLabel, 0, wxRIGHT, 8);
    wxTextCtrl* passCtrl = new wxTextCtrl(panel, wxID_ANY, "", wxDefaultPosition, wxDefaultSize, wxTE_PASSWORD);
    hbox3->Add(passCtrl, 1);
    vbox->Add(hbox3, 0, wxEXPAND | wxLEFT | wxRIGHT | wxTOP, 10);

    // Buttons
    wxBoxSizer* hbox4 = new wxBoxSizer(wxHORIZONTAL);
    wxButton* addButton = new wxButton(panel, wxID_OK, wxT("Add"));
    wxButton* cancelButton = new wxButton(panel, wxID_CANCEL, wxT("Cancel"));
    hbox4->Add(addButton, 1);
    hbox4->Add(cancelButton, 1, wxLEFT, 5);
    vbox->Add(hbox4, 0, wxALIGN_CENTER | wxTOP | wxBOTTOM, 10);

    panel->SetSizer(vbox);

    if (dialog->ShowModal() == wxID_OK) {
        wxString service = serviceCtrl->GetValue();
        wxString username = userCtrl->GetValue();
        wxString password = passCtrl->GetValue();

        if (service.IsEmpty() || username.IsEmpty() || password.IsEmpty()) {
            wxMessageBox("All fields are required.", "Error", wxOK | wxICON_ERROR);
            dialog->Destroy();
            return;
        }

        try {
            passwordManager.addNewPassword(std::string(service.mb_str()), std::string(username.mb_str()), std::string(password.mb_str()));
            wxMessageBox("Password added successfully.", "Info", wxOK | wxICON_INFORMATION);
        }
        catch (const std::exception& e) {
            wxMessageBox(e.what(), "Error", wxOK | wxICON_ERROR);
        }
    }

    dialog->Destroy();
}

void MainMenuFrame::OnGeneratePassword(wxCommandEvent& event) {
    wxDialog* dialog = new wxDialog(this, wxID_ANY, "Generate a Password", wxDefaultPosition, wxSize(400, 250));
    wxPanel* panel = new wxPanel(dialog, wxID_ANY);
    wxBoxSizer* vbox = new wxBoxSizer(wxVERTICAL);

    // Password Length
    wxBoxSizer* hbox1 = new wxBoxSizer(wxHORIZONTAL);
    wxStaticText* lengthLabel = new wxStaticText(panel, wxID_ANY, wxT("Password Length:"));
    hbox1->Add(lengthLabel, 0, wxRIGHT, 8);
    wxTextCtrl* lengthCtrl = new wxTextCtrl(panel, wxID_ANY);
    hbox1->Add(lengthCtrl, 1);
    vbox->Add(hbox1, 0, wxEXPAND | wxLEFT | wxRIGHT | wxTOP, 10);

    // Buttons
    wxBoxSizer* hbox2 = new wxBoxSizer(wxHORIZONTAL);
    wxButton* generateButton = new wxButton(panel, wxID_OK, wxT("Generate"));
    wxButton* cancelButton = new wxButton(panel, wxID_CANCEL, wxT("Cancel"));
    hbox2->Add(generateButton, 1);
    hbox2->Add(cancelButton, 1, wxLEFT, 5);
    vbox->Add(hbox2, 0, wxALIGN_CENTER | wxTOP | wxBOTTOM, 10);

    // Generated Password Display
    wxStaticText* generatedLabel = new wxStaticText(panel, wxID_ANY, wxT("Generated Password:"));
    vbox->Add(generatedLabel, 0, wxLEFT | wxTOP, 10);
    wxTextCtrl* generatedCtrl = new wxTextCtrl(panel, wxID_ANY, "", wxDefaultPosition, wxDefaultSize, wxTE_READONLY);
    vbox->Add(generatedCtrl, 0, wxEXPAND | wxLEFT | wxRIGHT | wxTOP, 10);

    panel->SetSizer(vbox);

    if (dialog->ShowModal() == wxID_OK) {
        wxString lengthStr = lengthCtrl->GetValue();
        long length;
        if (!lengthStr.ToLong(&length) || length <= 0) {
            wxMessageBox("Please enter a valid positive number for length.", "Error", wxOK | wxICON_ERROR);
            dialog->Destroy();
            return;
        }

        try {
            std::string generatedPassword = passwordManager.generatePassword(static_cast<int>(length));
            generatedCtrl->SetValue(generatedPassword);
            wxMessageBox("Password generated successfully.", "Info", wxOK | wxICON_INFORMATION);

            // Optionally, ask to save the generated password
            int res = wxMessageBox("Do you want to save this password?", "Save Password", wxYES_NO | wxICON_QUESTION);
            if (res == wxYES) {
                // Prompt for service name and username
                wxDialog* saveDialog = new wxDialog(this, wxID_ANY, "Save Generated Password", wxDefaultPosition, wxSize(400, 300));
                wxPanel* savePanel = new wxPanel(saveDialog, wxID_ANY);
                wxBoxSizer* saveVbox = new wxBoxSizer(wxVERTICAL);

                // Service Name
                wxBoxSizer* saveHbox1 = new wxBoxSizer(wxHORIZONTAL);
                wxStaticText* serviceLabel = new wxStaticText(savePanel, wxID_ANY, wxT("Service Name:"));
                saveHbox1->Add(serviceLabel, 0, wxRIGHT, 8);
                wxTextCtrl* serviceCtrl = new wxTextCtrl(savePanel, wxID_ANY);
                saveHbox1->Add(serviceCtrl, 1);
                saveVbox->Add(saveHbox1, 0, wxEXPAND | wxLEFT | wxRIGHT | wxTOP, 10);

                // Service Username
                wxBoxSizer* saveHbox2 = new wxBoxSizer(wxHORIZONTAL);
                wxStaticText* userLabel = new wxStaticText(savePanel, wxID_ANY, wxT("Service Username:"));
                saveHbox2->Add(userLabel, 0, wxRIGHT, 8);
                wxTextCtrl* userCtrl = new wxTextCtrl(savePanel, wxID_ANY);
                saveHbox2->Add(userCtrl, 1);
                saveVbox->Add(saveHbox2, 0, wxEXPAND | wxLEFT | wxRIGHT | wxTOP, 10);

                // Buttons
                wxBoxSizer* saveHbox3 = new wxBoxSizer(wxHORIZONTAL);
                wxButton* saveButton = new wxButton(savePanel, wxID_OK, wxT("Save"));
                wxButton* saveCancelButton = new wxButton(savePanel, wxID_CANCEL, wxT("Cancel"));
                saveHbox3->Add(saveButton, 1);
                saveHbox3->Add(saveCancelButton, 1, wxLEFT, 5);
                saveVbox->Add(saveHbox3, 0, wxALIGN_CENTER | wxTOP | wxBOTTOM, 10);

                savePanel->SetSizer(saveVbox);

                if (saveDialog->ShowModal() == wxID_OK) {
                    wxString service = serviceCtrl->GetValue();
                    wxString username = userCtrl->GetValue();

                    if (service.IsEmpty() || username.IsEmpty()) {
                        wxMessageBox("All fields are required.", "Error", wxOK | wxICON_ERROR);
                        saveDialog->Destroy();
                        dialog->Destroy();
                        return;
                    }

                    try {
                        passwordManager.addNewPassword(std::string(service.mb_str()), std::string(username.mb_str()), generatedPassword);
                        wxMessageBox("Generated password saved successfully.", "Info", wxOK | wxICON_INFORMATION);
                    }
                    catch (const std::exception& e) {
                        wxMessageBox(e.what(), "Error", wxOK | wxICON_ERROR);
                    }
                }

                saveDialog->Destroy();
            }
        }
        catch (const std::exception& e) {
            wxMessageBox(e.what(), "Error", wxOK | wxICON_ERROR);
        }
    }

    dialog->Destroy();
}

void MainMenuFrame::OnRetrievePassword(wxCommandEvent& event) {
    wxDialog* dialog = new wxDialog(this, wxID_ANY, "Retrieve a Password", wxDefaultPosition, wxSize(400, 200));
    wxPanel* panel = new wxPanel(dialog, wxID_ANY);
    wxBoxSizer* vbox = new wxBoxSizer(wxVERTICAL);

    // Service Name
    wxBoxSizer* hbox1 = new wxBoxSizer(wxHORIZONTAL);
    wxStaticText* serviceLabel = new wxStaticText(panel, wxID_ANY, wxT("Service Name:"));
    hbox1->Add(serviceLabel, 0, wxRIGHT, 8);
    wxTextCtrl* serviceCtrl = new wxTextCtrl(panel, wxID_ANY);
    hbox1->Add(serviceCtrl, 1);
    vbox->Add(hbox1, 0, wxEXPAND | wxLEFT | wxRIGHT | wxTOP, 10);

    // Buttons
    wxBoxSizer* hbox2 = new wxBoxSizer(wxHORIZONTAL);
    wxButton* retrieveButton = new wxButton(panel, wxID_OK, wxT("Retrieve"));
    wxButton* cancelButton = new wxButton(panel, wxID_CANCEL, wxT("Cancel"));
    hbox2->Add(retrieveButton, 1);
    hbox2->Add(cancelButton, 1, wxLEFT, 5);
    vbox->Add(hbox2, 0, wxALIGN_CENTER | wxTOP | wxBOTTOM, 10);

    // Retrieved Password Display
    wxStaticText* retrievedLabel = new wxStaticText(panel, wxID_ANY, wxT("Password:"));
    vbox->Add(retrievedLabel, 0, wxLEFT | wxTOP, 10);
    wxTextCtrl* retrievedCtrl = new wxTextCtrl(panel, wxID_ANY, "", wxDefaultPosition, wxDefaultSize, wxTE_READONLY);
    vbox->Add(retrievedCtrl, 0, wxEXPAND | wxLEFT | wxRIGHT | wxTOP, 10);

    panel->SetSizer(vbox);

    if (dialog->ShowModal() == wxID_OK) {
        wxString service = serviceCtrl->GetValue();

        if (service.IsEmpty()) {
            wxMessageBox("Service name is required.", "Error", wxOK | wxICON_ERROR);
            dialog->Destroy();
            return;
        }

        try {
            auto credential = passwordManager.getCredential(std::string(service.mb_str()));
            if (credential.has_value()) {
                // Decrypt the password before displaying
                std::string passwordHex = credential.value();
                std::vector<unsigned char> encryptedPassword;

                // Convert hex to binary
                for (size_t i = 0; i < passwordHex.size(); i += 2) {
                    unsigned char byte = static_cast<unsigned char>(std::stoi(passwordHex.substr(i, 2), nullptr, 16));
                    encryptedPassword.push_back(byte);
                }

                // Decrypt password
                std::string decryptedPassword = EncryptionNS::decrypt(encryptedPassword, passwordManager.encryptionKey);

                retrievedCtrl->SetValue(decryptedPassword);
                wxMessageBox("Password retrieved successfully.", "Info", wxOK | wxICON_INFORMATION);
            }
            else {
                wxMessageBox("Service not found.", "Error", wxOK | wxICON_ERROR);
            }
        }
        catch (const std::exception& e) {
            wxMessageBox(e.what(), "Error", wxOK | wxICON_ERROR);
        }
    }

    dialog->Destroy();
}

void MainMenuFrame::OnDeletePassword(wxCommandEvent& event) {
    wxDialog* dialog = new wxDialog(this, wxID_ANY, "Delete a Password", wxDefaultPosition, wxSize(400, 200));
    wxPanel* panel = new wxPanel(dialog, wxID_ANY);
    wxBoxSizer* vbox = new wxBoxSizer(wxVERTICAL);

    // Service Name
    wxBoxSizer* hbox1 = new wxBoxSizer(wxHORIZONTAL);
    wxStaticText* serviceLabel = new wxStaticText(panel, wxID_ANY, wxT("Service Name:"));
    hbox1->Add(serviceLabel, 0, wxRIGHT, 8);
    wxTextCtrl* serviceCtrl = new wxTextCtrl(panel, wxID_ANY);
    hbox1->Add(serviceCtrl, 1);
    vbox->Add(hbox1, 0, wxEXPAND | wxLEFT | wxRIGHT | wxTOP, 10);

    // Buttons
    wxBoxSizer* hbox2 = new wxBoxSizer(wxHORIZONTAL);
    wxButton* deleteButton = new wxButton(panel, wxID_OK, wxT("Delete"));
    wxButton* cancelButton = new wxButton(panel, wxID_CANCEL, wxT("Cancel"));
    hbox2->Add(deleteButton, 1);
    hbox2->Add(cancelButton, 1, wxLEFT, 5);
    vbox->Add(hbox2, 0, wxALIGN_CENTER | wxTOP | wxBOTTOM, 10);

    panel->SetSizer(vbox);

    if (dialog->ShowModal() == wxID_OK) {
        wxString service = serviceCtrl->GetValue();

        if (service.IsEmpty()) {
            wxMessageBox("Service name is required.", "Error", wxOK | wxICON_ERROR);
            dialog->Destroy();
            return;
        }

        try {
            passwordManager.deletePassword(std::string(service.mb_str()));
            wxMessageBox("Password deleted successfully.", "Info", wxOK | wxICON_INFORMATION);
        }
        catch (const std::exception& e) {
            wxMessageBox(e.what(), "Error", wxOK | wxICON_ERROR);
        }
    }

    dialog->Destroy();
}

void MainMenuFrame::OnViewAllPasswords(wxCommandEvent& event) {
    wxDialog* dialog = new wxDialog(this, wxID_ANY, "All Stored Passwords", wxDefaultPosition, wxSize(600, 400));
    wxPanel* panel = new wxPanel(dialog, wxID_ANY);
    wxBoxSizer* vbox = new wxBoxSizer(wxVERTICAL);

    // Create a multi-line read-only text control to display passwords
    wxTextCtrl* passwordsCtrl = new wxTextCtrl(panel, wxID_ANY, "", wxDefaultPosition, wxSize(580, 340), wxTE_MULTILINE | wxTE_READONLY);
    
    std::ostringstream oss;
    std::vector<std::pair<std::string, std::string>> decryptedCredentials = passwordManager.getAllDecryptedCredentials(); // Using the decrypted credentials

    if (decryptedCredentials.empty()) {
        oss << "No passwords stored.\n";
    }
    else {
        oss << std::left << std::setw(20) << "Service"
            << std::setw(20) << "Username"
            << "Password\n";
        oss << "------------------------------------------------------------\n";
        for (const auto& entry : decryptedCredentials) {
            std::string service = entry.first;
            std::string username_password = entry.second;

            size_t delimiter_pos = username_password.find(':');
            std::string username = (delimiter_pos != std::string::npos) ? username_password.substr(0, delimiter_pos) : username_password;
            std::string password = (delimiter_pos != std::string::npos) ? username_password.substr(delimiter_pos + 1) : "";

            oss << std::left << std::setw(20) << service
                << std::setw(20) << username
                << password << "\n";
        }
    }

    passwordsCtrl->SetValue(oss.str());

    vbox->Add(passwordsCtrl, 1, wxEXPAND | wxALL, 10);

    // Close Button
    wxButton* closeButton = new wxButton(panel, wxID_OK, wxT("Close"));
    vbox->Add(closeButton, 0, wxALIGN_CENTER | wxTOP | wxBOTTOM, 10);

    panel->SetSizer(vbox);

    dialog->ShowModal();
    dialog->Destroy();
}

    passwordsCtrl->SetValue(oss.str());

    vbox->Add(passwordsCtrl, 1, wxEXPAND | wxALL, 10);

    // Close Button
    wxButton* closeButton = new wxButton(panel, wxID_OK, wxT("Close"));
    vbox->Add(closeButton, 0, wxALIGN_CENTER | wxTOP | wxBOTTOM, 10);

    panel->SetSizer(vbox);

    dialog->ShowModal();
    dialog->Destroy();
}

void MainMenuFrame::OnExit(wxCommandEvent& event) {
    try {
        passwordManager.handleExit();
    }
    catch (const std::exception& e) {
        wxMessageBox(e.what(), "Error", wxOK | wxICON_ERROR);
    }
    Close(true);
}

} // namespace PasswordNS
