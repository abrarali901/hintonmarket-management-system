#ifndef MARKETDATE_H
#define MARKETDATE_H

#include <QDate>
#include <QVector>

class StallBooking;
class WaitlistEntry;

/**
 * @class MarketDate
 * @brief Represents a single Sunday market date.
 *
 * Tracks stall capacity (max 2 food + 2 artisan for D1/D2 testing),
 * current bookings, and the waitlist for this date.
 *
 * @author Victor
 */
class MarketDate {
public:
    MarketDate();
    MarketDate(const QDate& date);
    ~MarketDate();

    int getId() const;
    QDate getDate() const;
    QString getDateString() const;

    // Stall capacity
    int getMaxFoodStalls() const;
    int getMaxArtisanStalls() const;
    int getBookedFoodStalls() const;
    int getBookedArtisanStalls() const;
    int getAvailableFoodStalls() const;
    int getAvailableArtisanStalls() const;

    void setId(int id);
    void setDate(const QDate& date);

    // Booking management for this date
    QVector<StallBooking*>& getBookings();
    void addBooking(StallBooking* booking);
    void removeBooking(StallBooking* booking);

    // Waitlist management for this date
    QVector<WaitlistEntry*>& getWaitlistEntries();
    void addWaitlistEntry(WaitlistEntry* entry);
    void removeWaitlistEntry(WaitlistEntry* entry);

    bool hasFoodStallAvailable() const;
    bool hasArtisanStallAvailable() const;

private:
    int m_id;
    QDate m_date;
    int m_maxFoodStalls;      // default 2 for testing
    int m_maxArtisanStalls;   // default 2 for testing
    QVector<StallBooking*> m_bookings;
    QVector<WaitlistEntry*> m_waitlistEntries;
};

#endif // MARKETDATE_H
