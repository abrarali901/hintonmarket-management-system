#ifndef VENDORDASHBOARDVIEW_H
#define VENDORDASHBOARDVIEW_H

#include <QWidget>

class QLabel;
class QTableWidget;
class QListWidget;
class QPushButton;
class QScrollArea;
class DashboardController;
class BookingController;
class WaitlistController;
class Vendor;

/**
 * @class VendorDashboardView
 * @brief Main vendor dashboard with business info, compliance,
 * bookings, waitlist, and notifications sections.
 * @author Osasuyi
 */
class VendorDashboardView : public QWidget {
    Q_OBJECT

public:
    explicit VendorDashboardView(Vendor* vendor, QWidget *parent = nullptr);
    ~VendorDashboardView();

    void refresh();

signals:
    void browseMarketRequested();
    void logoutRequested();

private slots:
    void onBrowseMarketClicked();
    void onLogoutClicked();
    void onCancelBookingClicked();
    void onCancelWaitlistClicked();

private:
    // Business info labels
    QLabel* m_businessNameLabel;
    QLabel* m_ownerNameLabel;
    QLabel* m_categoryLabel;
    QLabel* m_contactLabel;
    QLabel* m_addressLabel;

    // Tables
    QTableWidget* m_complianceTable;
    QTableWidget* m_bookingsTable;
    QTableWidget* m_waitlistTable;
    QListWidget* m_notificationsList;

    // Buttons
    QPushButton* m_browseMarketButton;
    QPushButton* m_cancelBookingButton;
    QPushButton* m_cancelWaitlistButton;
    QPushButton* m_logoutButton;

    // Current vendor
    Vendor* m_vendor;

    // Controllers
    DashboardController* m_dashboardController;
    BookingController* m_bookingController;
    WaitlistController* m_waitlistController;

    void setupUI();
    void loadBusinessInfo();
    void loadComplianceDocs();
    void loadBookings();
    void loadWaitlistEntries();
    void loadNotifications();
};

#endif // VENDORDASHBOARDVIEW_H
