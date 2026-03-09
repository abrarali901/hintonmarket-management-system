#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class QStackedWidget;
class LoginView;
class VendorDashboardView;
class MarketScheduleView;
class AuthenticationController;
class Vendor;

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public slots:
    void showLogin();
    void showVendorDashboard(Vendor* vendor);
    void showMarketSchedule(Vendor* vendor);
    void showOperatorView();
    void showAdminView();

private:
    QStackedWidget* m_stackedWidget;
    LoginView* m_loginView;
    VendorDashboardView* m_dashboardView;
    MarketScheduleView* m_scheduleView;
    QWidget* m_operatorView;
    QWidget* m_adminView;

    AuthenticationController* m_authController;

    void setupUI();
    void clearViews();
};

#endif // MAINWINDOW_H
