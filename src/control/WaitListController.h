#ifndef WAITLISTCONTROLLER_H
#define WAITLISTCONTROLLER_H

#include <QVector>

class Vendor;
class MarketDate;
class WaitlistEntry;

class WaitlistController {
public:
    WaitlistController();
    ~WaitlistController();

    // Join waitlist - returns true if successful
    bool joinWaitlist(Vendor* vendor, MarketDate* date);

    // Leave waitlist - returns true if successful
    bool leaveWaitlist(WaitlistEntry* entry);

    // Get waitlist entries for a market date and vendor category
    QVector<WaitlistEntry*> getWaitlistForDate(MarketDate* date, Vendor::VendorCategory category);

    // Get vendor's position in waitlist (1-based, 0 if not found)
    int getPosition(Vendor* vendor, MarketDate* date);

    // Called when a stall becomes available (after cancellation)
    void notifyNextInQueue(MarketDate* date, Vendor::VendorCategory category);

private:
    // Helper to update positions after removal
    void updatePositions(MarketDate* date, Vendor::VendorCategory category);
};

#endif // WAITLISTCONTROLLER_H