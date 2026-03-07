#ifndef WAITLISTENTRY_H
#define WAITLISTENTRY_H

#include <QDateTime>

class Vendor;
class MarketDate;

class WaitlistEntry {
public:
    WaitlistEntry();
    WaitlistEntry(Vendor* vendor, MarketDate* marketDate);
    ~WaitlistEntry();

    // Getters
    Vendor* getVendor() const;
    MarketDate* getMarketDate() const;
    QDateTime getJoinTime() const;
    int getPosition() const;

    // Setters
    void setVendor(Vendor* vendor);
    void setMarketDate(MarketDate* marketDate);
    void setPosition(int position);

private:
    Vendor* m_vendor;
    MarketDate* m_marketDate;
    // when the vendor joins he waitlist
    QDateTime m_joinTime;
    // queue position 
    int m_position;
};

#endif // WAITLISTENTRY_H