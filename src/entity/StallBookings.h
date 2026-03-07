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

    // Getters
    Vendor* getVendor() const;
    MarketDate* getMarketDate() const;
    QDateTime getBookingTime() const;

    // Setters
    void setVendor(Vendor* vendor);
    void setMarketDate(MarketDate* marketDate);

private:
    /*
    Note: 
    Using pointers to reference existing Vendor and MarketDate objects
    stored in DataManager. No copies made - just stores memory addresses.
    Changes to the original objects are reflected here automatically.
    */
    Vendor* m_vendor;
    MarketDate* m_marketDate;
    QDateTime m_bookingTime;
};

#endif // STALLBOOKING_H