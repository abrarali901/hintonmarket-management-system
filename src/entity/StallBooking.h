#ifndef STALLBOOKING_H
#define STALLBOOKING_H

#include <QDateTime>

class Vendor;
class MarketDate;

class StallBooking {
public:
    StallBooking();
    StallBooking(Vendor* vendor, MarketDate* marketDate);
    ~StallBooking();

    Vendor* getVendor() const;
    MarketDate* getMarketDate() const;
    QDateTime getBookingTime() const;

    void setVendor(Vendor* vendor);
    void setMarketDate(MarketDate* marketDate);

private:
    Vendor* m_vendor;
    MarketDate* m_marketDate;
    QDateTime m_bookingTime;
};

#endif // STALLBOOKING_H
