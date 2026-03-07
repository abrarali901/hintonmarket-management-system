#ifndef DASHBOARDCONTROLLER_H
#define DASHBOARDCONTROLLER_H

#include <QVector>

class Vendor;
class StallBooking;
class WaitlistEntry;
class ComplianceDocument;
class Notification;

class DashboardController {
public:
    DashboardController();
    ~DashboardController();

    // Set current vendor for dashboard
    void setVendor(Vendor* vendor);
    Vendor* getVendor() const;

    // Get vendor's data for display
    QVector<StallBooking*> getBookings();
    QVector<WaitlistEntry*> getWaitlistEntries();
    QVector<ComplianceDocument*> getComplianceDocuments();
    QVector<Notification*> getNotifications();
    QVector<Notification*> getUnreadNotifications();

    // Mark notification as read
    void markNotificationRead(Notification* notification);

private:
    Vendor* m_vendor;
};

#endif // DASHBOARDCONTROLLER_H