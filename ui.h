// ui.h
#ifndef UI_H
#define UI_H

#include <wx/wx.h>
#include "manager.h"

namespace PasswordNS {

// Forward declaration
class PasswordManager;

// Application Class
class PasswordManagerApp : public wxApp {
public:
    virtual bool OnInit();
};

// Login/Register Frame
class LoginFrame : public wxFrame {
public:
    LoginFrame(const wxString& title, PasswordManager& manager);

private:
    PasswordManager& passwordManager;
    wxTextCtrl* usernameCtrl;
    wxTextCtrl* passwordCtrl;
    wxButton* loginButton;
    wxButton* registerButton;
    wxStaticText* message;

    void OnLogin(wxCommandEvent& event);
    void OnRegister(wxCommandEvent& event);
};

// Main Menu Frame
class MainMenuFrame : public wxFrame {
public:
    MainMenuFrame(const wxString& title, PasswordManager& manager);

private:
    PasswordManager& passwordManager;
    wxButton* addPasswordButton;
    wxButton* generatePasswordButton;
    wxButton* retrievePasswordButton;
    wxButton* deletePasswordButton;
    wxButton* viewAllPasswordsButton;
    wxButton* exitButton;
    wxStaticText* message;

    void OnAddPassword(wxCommandEvent& event);
    void OnGeneratePassword(wxCommandEvent& event);
    void OnRetrievePassword(wxCommandEvent& event);
    void OnDeletePassword(wxCommandEvent& event);
    void OnViewAllPasswords(wxCommandEvent& event);
    void OnExit(wxCommandEvent& event);
};

} // namespace PasswordNS

#endif // UI_H
