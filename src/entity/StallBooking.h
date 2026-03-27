#ifndef STALLBOOKING_H
#define STALLBOOKING_H

#include <QDateTime>

class Vendor;
class MarketDate;

/**
 * @class StallBooking
 * @brief Links a vendor to a market date booking.
 *
 * Created when a vendor (or operator on their behalf) books a stall.
 * Business rule: one active booking per vendor at a time.
 *
 * @author Sheng
 */
class StallBooking {
public:
    StallBooking();
    StallBooking(Vendor* vendor, MarketDate* marketDate);
    ~StallBooking();

    int getId() const;
    Vendor* getVendor() const;
    MarketDate* getMarketDate() const;
    QDateTime getBookingTime() const;

    void setId(int id);
    void setVendor(Vendor* vendor);
    void setMarketDate(MarketDate* marketDate);
    void setBookingTime(const QDateTime& time);

private:
    int m_id;
    Vendor* m_vendor;
    MarketDate* m_marketDate;
    QDateTime m_bookingTime;
};

#endif // STALLBOOKING_H
