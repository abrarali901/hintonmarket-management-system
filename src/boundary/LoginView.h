#ifndef LOGINVIEW_H
#define LOGINVIEW_H

#include <QWidget>

class QLineEdit;
class QPushButton;
class QLabel;
class AuthenticationController;
class User;

/**
 * @class LoginView
 * @brief Login screen - verifies username against DataManager.
 * @author Sheng
 */
class LoginView : public QWidget {
    Q_OBJECT

public:
    explicit LoginView(QWidget *parent = nullptr);
    ~LoginView();

    void clearFields();

signals:
    void loginSuccessful(User* user);

private slots:
    void onLoginClicked();

private:
    QLineEdit* m_usernameInput;
    QPushButton* m_loginButton;
    QLabel* m_errorLabel;
    QLabel* m_titleLabel;
    QLabel* m_helpLabel;

    AuthenticationController* m_authController;

    void setupUI();
};

#endif // LOGINVIEW_H
