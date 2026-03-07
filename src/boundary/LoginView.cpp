#include "LoginView.h"
#include "AuthenticationController.h"
#include "User.h"

#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QVBoxLayout>

LoginView::LoginView(QWidget *parent)
    : QWidget(parent),
      m_usernameInput(nullptr),
      m_loginButton(nullptr),
      m_errorLabel(nullptr),
      m_authController(new AuthenticationController()) {
    setupUI();
}

LoginView::~LoginView() {
    delete m_authController;
}

void LoginView::setupUI() {
    // TODO: Create and arrange UI elements
    // - Title label
    // - Username input field
    // - Login button
    // - Error label (hidden by default)

    QVBoxLayout* layout = new QVBoxLayout(this);

    m_usernameInput = new QLineEdit(this);
    m_usernameInput->setPlaceholderText("Enter username");

    m_loginButton = new QPushButton("Login", this);

    m_errorLabel = new QLabel(this);
    m_errorLabel->setStyleSheet("color: red;");
    m_errorLabel->hide();

    layout->addWidget(m_usernameInput);
    layout->addWidget(m_loginButton);
    layout->addWidget(m_errorLabel);

    connect(m_loginButton, &QPushButton::clicked, this, &LoginView::onLoginClicked);
}

void LoginView::onLoginClicked() {
    QString username = m_usernameInput->text().trimmed();

    if (username.isEmpty()) {
        m_errorLabel->setText("Please enter a username");
        m_errorLabel->show();
        return;
    }

    User* user = m_authController->login(username);

    if (!user) {
        m_errorLabel->setText("User not found");
        m_errorLabel->show();
        return;
    }

    m_errorLabel->hide();

    // TODO: Navigate to appropriate view based on user type
    // if (m_authController->isCurrentUserVendor()) { ... }
    // if (m_authController->isCurrentUserOperator()) { ... }
    // if (m_authController->isCurrentUserAdmin()) { ... }
}