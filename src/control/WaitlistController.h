#ifndef WAITLISTCONTROLLER_H
#define WAITLISTCONTROLLER_H

#include <QVector>
#include "Vendor.h"

class MarketDate;
class WaitlistEntry;

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
