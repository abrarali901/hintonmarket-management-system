#ifndef MARKETSCHEDULECONTROLLER_H
#define MARKETSCHEDULECONTROLLER_H

#include <QVector>

class MarketDate;
class Vendor;

/**
 * @class MarketScheduleController
 * @brief Provides market date browsing functionality.
 *
 * Filters market dates to show only those within the 4-week
 * booking window. Checks stall availability for a given vendor's category.
 *
 * @author Sheng
 */
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
