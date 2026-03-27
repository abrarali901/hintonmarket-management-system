#ifndef OPERATORDASHBOARDVIEW_H
#define OPERATORDASHBOARDVIEW_H

#include <QWidget>

class QLabel;
class QComboBox;
class QTableWidget;
class QPushButton;
class QGroupBox;
class OperatorController;
class MarketScheduleController;
class Vendor;
class MarketDate;

/**
 * @class OperatorDashboardView
 * @brief Market Operator UI for managing vendor bookings/waitlists (D2).
 * Select a vendor from dropdown, then book/cancel/remove-waitlist.
 * @author Victor
 */
class OperatorDashboardView : public QWidget {
    Q_OBJECT

public:
    explicit OperatorDashboardView(QWidget *parent = nullptr);
    ~OperatorDashboardView();

signals:
    void logoutRequested();

private slots:
    void onVendorSelected(int index);
    void onBookStallClicked();
    void onCancelBookingClicked();
    void onRemoveFromWaitlistClicked();
    void onLogoutClicked();

private:
    // Vendor selection
    QComboBox* m_vendorComboBox;
    QLabel* m_vendorInfoLabel;

    // Market schedule table
    QTableWidget* m_scheduleTable;

    // Vendor's current bookings table
    QTableWidget* m_bookingsTable;

    // Vendor's current waitlist table
    QTableWidget* m_waitlistTable;

    // Action buttons
    QPushButton* m_bookStallButton;
    QPushButton* m_cancelBookingButton;
    QPushButton* m_removeWaitlistButton;
    QPushButton* m_logoutButton;

    // Status feedback
    QLabel* m_statusLabel;

    // Controllers
    OperatorController* m_operatorController;
    MarketScheduleController* m_scheduleController;

    // Currently selected vendor
    Vendor* m_selectedVendor;

    void setupUI();
    void refreshAll();
    void refreshScheduleTable();
    void refreshBookingsTable();
    void refreshWaitlistTable();
    void refreshVendorInfo();
};

#endif // OPERATORDASHBOARDVIEW_H
