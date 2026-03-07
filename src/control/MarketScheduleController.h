#ifndef MARKETSCHEDULECONTROLLER_H
#define MARKETSCHEDULECONTROLLER_H

#include <QVector>

class MarketDate;
class Vendor;

class MarketScheduleController {
public:
    MarketScheduleController();
    ~MarketScheduleController();

    // Get all market dates
    QVector<MarketDate*> getAllMarketDates();

    // Get available market dates (4 weeks from now)
    QVector<MarketDate*> getAvailableMarketDates();

    // Check stall availability for a vendor category
    bool hasAvailableStall(MarketDate* date, Vendor* vendor);

private:
    // Helper to check if date is within booking window
    bool isWithinBookingWindow(MarketDate* date);
};

#endif // MARKETSCHEDULECONTROLLER_H