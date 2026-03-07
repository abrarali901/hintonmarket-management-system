#ifndef LOGINVIEW_H
#define LOGINVIEW_H

#include <QWidget>

class QLineEdit;
class QPushButton;
class QLabel;
class AuthenticationController;

class LoginView : public QWidget {
    // Qt marco needed for signals/slots
    Q_OBJECT

public:
    explicit LoginView(QWidget *parent = nullptr);
    ~LoginView();

private slots:
    void onLoginClicked();

private:
    // UI elements
    QLineEdit* m_usernameInput;
    QPushButton* m_loginButton;
    QLabel* m_errorLabel;

    // Controller
    AuthenticationController* m_authController;

    // Setup UI
    void setupUI();
};

#endif // LOGINVIEW_H