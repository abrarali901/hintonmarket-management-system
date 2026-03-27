#ifndef OPERATORCONTROLLER_H
#define OPERATORCONTROLLER_H

#include <QVector>

class Vendor;
class MarketDate;
class StallBooking;
class WaitlistEntry;
class BookingController;
class WaitlistController;

/**
 * @class OperatorController
 * @brief Market Operator actions on behalf of vendors (D2 feature).
 *
 * Delegates to BookingController and WaitlistController so the same
 * business rules apply whether a vendor acts directly or the operator
 * acts on their behalf.
 *
 * Three features:
 *  1. Book a stall for a vendor
 *  2. Cancel a vendor's booking
 *  3. Remove a vendor from a waitlist
 *
 * @author Victor
 */
class OperatorController {
public:
    OperatorController();
    ~OperatorController();

    bool bookStallForVendor(Vendor* vendor, MarketDate* date);
    bool cancelBookingForVendor(StallBooking* booking);
    bool removeVendorFromWaitlist(WaitlistEntry* entry);

    QVector<Vendor*> getAllVendors();
    QVector<MarketDate*> getAllMarketDates();

private:
    BookingController* m_bookingController;
    WaitlistController* m_waitlistController;
};

#endif // OPERATORCONTROLLER_H
