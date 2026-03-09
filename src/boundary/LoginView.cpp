#include "LoginView.h"
#include "AuthenticationController.h"
#include "User.h"

#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>

LoginView::LoginView(QWidget *parent)
    : QWidget(parent),
      m_usernameInput(nullptr),
      m_loginButton(nullptr),
      m_errorLabel(nullptr),
      m_titleLabel(nullptr),
      m_helpLabel(nullptr),
      m_authController(new AuthenticationController()) {
    setupUI();
}

LoginView::~LoginView() {
    delete m_authController;
}

void LoginView::setupUI() {
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->setAlignment(Qt::AlignCenter);

    // Title
    m_titleLabel = new QLabel("HintonMarket", this);
    m_titleLabel->setStyleSheet(
        "font-size: 28px; font-weight: bold; color: #2c5f2d; padding: 10px;"
    );
    m_titleLabel->setAlignment(Qt::AlignCenter);

    QLabel* subtitle = new QLabel("Hintonville Farmers Market Management System", this);
    subtitle->setStyleSheet("font-size: 14px; color: #555; padding-bottom: 20px;");
    subtitle->setAlignment(Qt::AlignCenter);

    // Username input
    QLabel* usernameLabel = new QLabel("Enter your username:", this);
    usernameLabel->setStyleSheet("font-size: 13px; font-weight: bold;");

    m_usernameInput = new QLineEdit(this);
    m_usernameInput->setPlaceholderText("e.g., foodvendor1, artisanvendor1, operator1, admin1");
    m_usernameInput->setFixedWidth(400);
    m_usernameInput->setMinimumHeight(35);
    m_usernameInput->setStyleSheet("font-size: 13px; padding: 5px;");

    // Login button
    m_loginButton = new QPushButton("Login", this);
    m_loginButton->setFixedWidth(400);
    m_loginButton->setMinimumHeight(35);
    m_loginButton->setStyleSheet(
        "QPushButton { background-color: #2c5f2d; color: white; font-size: 14px; "
        "font-weight: bold; border-radius: 5px; }"
        "QPushButton:hover { background-color: #3a7a3d; }"
        "QPushButton:pressed { background-color: #1e4620; }"
    );

    // Error label
    m_errorLabel = new QLabel(this);
    m_errorLabel->setStyleSheet("color: red; font-size: 12px; padding: 5px;");
    m_errorLabel->setAlignment(Qt::AlignCenter);
    m_errorLabel->hide();

    // Help label showing available usernames
    m_helpLabel = new QLabel(
        "Available usernames:\n"
        "Food Vendors: foodvendor1, foodvendor2, foodvendor3, foodvendor4\n"
        "Artisan Vendors: artisanvendor1, artisanvendor2, artisanvendor3, artisanvendor4\n"
        "Market Operator: operator1  |  System Admin: admin1", this);
    m_helpLabel->setStyleSheet("font-size: 11px; color: #888; padding: 15px;");
    m_helpLabel->setAlignment(Qt::AlignCenter);

    mainLayout->addStretch();
    mainLayout->addWidget(m_titleLabel, 0, Qt::AlignCenter);
    mainLayout->addWidget(subtitle, 0, Qt::AlignCenter);
    mainLayout->addSpacing(10);
    mainLayout->addWidget(usernameLabel, 0, Qt::AlignCenter);
    mainLayout->addWidget(m_usernameInput, 0, Qt::AlignCenter);
    mainLayout->addWidget(m_loginButton, 0, Qt::AlignCenter);
    mainLayout->addWidget(m_errorLabel, 0, Qt::AlignCenter);
    mainLayout->addSpacing(10);
    mainLayout->addWidget(m_helpLabel, 0, Qt::AlignCenter);
    mainLayout->addStretch();

    connect(m_loginButton, &QPushButton::clicked, this, &LoginView::onLoginClicked);
    connect(m_usernameInput, &QLineEdit::returnPressed, this, &LoginView::onLoginClicked);
}

void LoginView::onLoginClicked() {
    QString username = m_usernameInput->text().trimmed();

    if (username.isEmpty()) {
        m_errorLabel->setText("Please enter a username.");
        m_errorLabel->show();
        return;
    }

    User* user = m_authController->login(username);

    if (!user) {
        m_errorLabel->setText("User not found. Please check the username and try again.");
        m_errorLabel->show();
        return;
    }

    m_errorLabel->hide();
    emit loginSuccessful(user);
}

void LoginView::clearFields() {
    m_usernameInput->clear();
    m_errorLabel->hide();
}
