#ifndef WAITLISTENTRY_H
#define WAITLISTENTRY_H

#include <QDateTime>

class Vendor;
class MarketDate;

/**
 * @class WaitlistEntry
 * @brief Tracks a vendor's position in a waitlist queue.
 *
 * Each waitlist is per-category per-market-date.
 * FIFO ordering: position 1 = first in line.
 *
 * @author Ali
 */
class WaitlistEntry {
public:
    WaitlistEntry();
    WaitlistEntry(Vendor* vendor, MarketDate* marketDate);
    ~WaitlistEntry();

    int getId() const;
    Vendor* getVendor() const;
    MarketDate* getMarketDate() const;
    QDateTime getJoinTime() const;
    int getPosition() const;

    void setId(int id);
    void setVendor(Vendor* vendor);
    void setMarketDate(MarketDate* marketDate);
    void setJoinTime(const QDateTime& time);
    void setPosition(int position);

private:
    int m_id;
    Vendor* m_vendor;
    MarketDate* m_marketDate;
    QDateTime m_joinTime;
    int m_position;  // FIFO queue position (1 = next)
};

#endif // WAITLISTENTRY_H
