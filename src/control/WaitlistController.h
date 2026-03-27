#ifndef WAITLISTCONTROLLER_H
#define WAITLISTCONTROLLER_H

#include <QVector>
#include "Vendor.h"

class MarketDate;
class WaitlistEntry;

/**
 * @class WaitlistController
 * @brief Manages waitlist join/leave and FIFO queue positions.
 *
 * Waitlists are per-category per-market-date. When a vendor leaves
 * or a stall opens up, positions get recalculated and the next
 * vendor in line is notified.
 * D2: persists all changes to SQLite via DatabaseManager.
 *
 * @author Ali, Osasuyi
 */
class WaitlistController {
public:
    WaitlistController();
    ~WaitlistController();

    bool joinWaitlist(Vendor* vendor, MarketDate* date);
    bool leaveWaitlist(WaitlistEntry* entry);

    QVector<WaitlistEntry*> getWaitlistForDate(MarketDate* date, Vendor::VendorCategory category);
    int getPosition(Vendor* vendor, MarketDate* date);
    void notifyNextInQueue(MarketDate* date, Vendor::VendorCategory category);

private:
    void updatePositions(MarketDate* date, Vendor::VendorCategory category);
};

#endif // WAITLISTCONTROLLER_H
