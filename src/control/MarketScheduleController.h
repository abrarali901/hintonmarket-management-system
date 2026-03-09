#ifndef MARKETSCHEDULECONTROLLER_H
#define MARKETSCHEDULECONTROLLER_H

#include <QVector>

class MarketDate;
class Vendor;

class MarketScheduleController {
public:
    MarketScheduleController();
    ~MarketScheduleController();

    QVector<MarketDate*> getAllMarketDates();
    QVector<MarketDate*> getAvailableMarketDates();
    bool hasAvailableStall(MarketDate* date, Vendor* vendor);

private:
    bool isWithinBookingWindow(MarketDate* date);
};

#endif // MARKETSCHEDULECONTROLLER_H
