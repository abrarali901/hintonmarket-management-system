#ifndef VENDORDASHBOARDVIEW_H
#define VENDORDASHBOARDVIEW_H

#include <QWidget>

class QLabel;
class QTableWidget;
class QListWidget;
class QPushButton;
class DashboardController;
class Vendor;

class VendorDashboardView : public QWidget {
    Q_OBJECT

public:
    explicit VendorDashboardView(Vendor* vendor, QWidget *parent = nullptr);
    ~VendorDashboardView();

    void refresh();

private slots:
    void onBrowseMarketClicked();
    void onLogoutClicked();

private:
    // UI elements
    QLabel* m_businessNameLabel;
    QLabel* m_categoryLabel;
    QLabel* m_contactLabel;
    QTableWidget* m_complianceTable;
    QTableWidget* m_bookingsTable;
    QTableWidget* m_waitlistTable;
    QListWidget* m_notificationsList;
    QPushButton* m_browseMarketButton;
    QPushButton* m_logoutButton;

    // Controller
    DashboardController* m_dashboardController;

    // Setup UI
    void setupUI();
    void loadBusinessInfo();
    void loadComplianceDocs();
    void loadBookings();
    void loadWaitlistEntries();
    void loadNotifications();
};

#endif // VENDORDASHBOARDVIEW_H