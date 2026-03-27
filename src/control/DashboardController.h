#ifndef DASHBOARDCONTROLLER_H
#define DASHBOARDCONTROLLER_H

#include <QVector>

class Vendor;
class StallBooking;
class WaitlistEntry;
class ComplianceDocument;
class Notification;

/**
 * @class DashboardController
 * @brief Provides data for the vendor status dashboard.
 *
 * Pulls bookings, waitlist entries, compliance docs and notifications
 * from the currently logged-in vendor object.
 *
 * @author Osasuyi
 */
class DashboardController {
public:
    DashboardController();
    ~DashboardController();

    void setVendor(Vendor* vendor);
    Vendor* getVendor() const;

    QVector<StallBooking*> getBookings();
    QVector<WaitlistEntry*> getWaitlistEntries();
    QVector<ComplianceDocument*> getComplianceDocuments();
    QVector<Notification*> getNotifications();
    QVector<Notification*> getUnreadNotifications();

    void markNotificationRead(Notification* notification);

private:
    Vendor* m_vendor;
};

#endif // DASHBOARDCONTROLLER_H
