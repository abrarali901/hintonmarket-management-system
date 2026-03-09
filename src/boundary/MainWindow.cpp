#include "MainWindow.h"
#include "LoginView.h"
#include "VendorDashboardView.h"
#include "MarketScheduleView.h"
#include "AuthenticationController.h"
#include "Vendor.h"
#include "User.h"

#include <QStackedWidget>
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
      m_stackedWidget(nullptr),
      m_loginView(nullptr),
      m_dashboardView(nullptr),
      m_scheduleView(nullptr),
      m_operatorView(nullptr),
      m_adminView(nullptr),
      m_authController(new AuthenticationController()) {
    setupUI();
}

MainWindow::~MainWindow() {
    delete m_authController;
}

void MainWindow::setupUI() {
    m_stackedWidget = new QStackedWidget(this);
    setCentralWidget(m_stackedWidget);

    // Create login view (index 0)
    m_loginView = new LoginView(this);
    m_stackedWidget->addWidget(m_loginView);

    // Connect login signal
    connect(m_loginView, &LoginView::loginSuccessful, this, [this](User* user) {
        if (user->getUserType() == User::UserType::VENDOR) {
            Vendor* vendor = static_cast<Vendor*>(user);
            showVendorDashboard(vendor);
        } else if (user->getUserType() == User::UserType::MARKET_OPERATOR) {
            showOperatorView();
        } else if (user->getUserType() == User::UserType::SYSTEM_ADMIN) {
            showAdminView();
        }
    });

    m_stackedWidget->setCurrentIndex(0);
    setWindowTitle("HintonMarket - Hintonville Farmers Market Management System");
    resize(900, 700);
}

void MainWindow::clearViews() {
    // Remove old views (except login at index 0)
    while (m_stackedWidget->count() > 1) {
        QWidget* w = m_stackedWidget->widget(1);
        m_stackedWidget->removeWidget(w);
        w->deleteLater();
    }
    m_dashboardView = nullptr;
    m_scheduleView = nullptr;
    m_operatorView = nullptr;
    m_adminView = nullptr;
}

void MainWindow::showLogin() {
    clearViews();
    m_loginView->clearFields();
    m_stackedWidget->setCurrentIndex(0);
    setWindowTitle("HintonMarket - Login");
}

void MainWindow::showVendorDashboard(Vendor* vendor) {
    clearViews();

    m_dashboardView = new VendorDashboardView(vendor, this);
    m_stackedWidget->addWidget(m_dashboardView);
    m_stackedWidget->setCurrentWidget(m_dashboardView);

    // Connect dashboard signals
    connect(m_dashboardView, &VendorDashboardView::browseMarketRequested, this, [this, vendor]() {
        showMarketSchedule(vendor);
    });

    connect(m_dashboardView, &VendorDashboardView::logoutRequested, this, &MainWindow::showLogin);

    setWindowTitle("HintonMarket - Vendor Dashboard: " + vendor->getBusinessName());
}

void MainWindow::showMarketSchedule(Vendor* vendor) {
    // Remove schedule view if it exists
    if (m_scheduleView) {
        m_stackedWidget->removeWidget(m_scheduleView);
        m_scheduleView->deleteLater();
        m_scheduleView = nullptr;
    }

    m_scheduleView = new MarketScheduleView(vendor, this);
    m_stackedWidget->addWidget(m_scheduleView);
    m_stackedWidget->setCurrentWidget(m_scheduleView);

    connect(m_scheduleView, &MarketScheduleView::backToDashboard, this, [this, vendor]() {
        showVendorDashboard(vendor);
    });

    setWindowTitle("HintonMarket - Market Schedule");
}

void MainWindow::showOperatorView() {
    clearViews();

    m_operatorView = new QWidget(this);
    QVBoxLayout* layout = new QVBoxLayout(m_operatorView);

    QLabel* title = new QLabel("Market Operator View", m_operatorView);
    title->setStyleSheet("font-size: 18px; font-weight: bold; padding: 20px;");
    title->setAlignment(Qt::AlignCenter);

    QLabel* info = new QLabel("Market Operator features are not implemented in Deliverable 1.\n"
                              "D1 focuses on Vendor functionality only.", m_operatorView);
    info->setAlignment(Qt::AlignCenter);
    info->setStyleSheet("font-size: 14px; color: #666; padding: 10px;");

    QPushButton* logoutBtn = new QPushButton("Logout", m_operatorView);
    logoutBtn->setFixedWidth(200);
    connect(logoutBtn, &QPushButton::clicked, this, &MainWindow::showLogin);

    layout->addStretch();
    layout->addWidget(title);
    layout->addWidget(info);
    layout->addWidget(logoutBtn, 0, Qt::AlignCenter);
    layout->addStretch();

    m_stackedWidget->addWidget(m_operatorView);
    m_stackedWidget->setCurrentWidget(m_operatorView);
    setWindowTitle("HintonMarket - Market Operator");
}

void MainWindow::showAdminView() {
    clearViews();

    m_adminView = new QWidget(this);
    QVBoxLayout* layout = new QVBoxLayout(m_adminView);

    QLabel* title = new QLabel("System Administrator View", m_adminView);
    title->setStyleSheet("font-size: 18px; font-weight: bold; padding: 20px;");
    title->setAlignment(Qt::AlignCenter);

    QLabel* info = new QLabel("System Administrator features are not implemented in Deliverable 1.\n"
                              "D1 focuses on Vendor functionality only.", m_adminView);
    info->setAlignment(Qt::AlignCenter);
    info->setStyleSheet("font-size: 14px; color: #666; padding: 10px;");

    QPushButton* logoutBtn = new QPushButton("Logout", m_adminView);
    logoutBtn->setFixedWidth(200);
    connect(logoutBtn, &QPushButton::clicked, this, &MainWindow::showLogin);

    layout->addStretch();
    layout->addWidget(title);
    layout->addWidget(info);
    layout->addWidget(logoutBtn, 0, Qt::AlignCenter);
    layout->addStretch();

    m_stackedWidget->addWidget(m_adminView);
    m_stackedWidget->setCurrentWidget(m_adminView);
    setWindowTitle("HintonMarket - System Administrator");
}
