#ifndef MARKETDATE_H
#define MARKETDATE_H

#include <QDate>
#include <QVector>

class StallBooking;
class WaitlistEntry;

class MarketDate {
public:
    // constructors
    MarketDate();
    MarketDate(const QDate& date);
    ~MarketDate();

    // Getters
    QDate getDate() const;
    QString getDateString() const;
    int getMaxFoodStalls() const;
    int getMaxArtisanStalls() const;
    int getBookedFoodStalls() const;
    int getBookedArtisanStalls() const;
    int getAvailableFoodStalls() const;
    int getAvailableArtisanStalls() const;

    // Setters
    void setDate(const QDate& date);

    // Bookings
    QVector<StallBooking*>& getBookings();
    void addBooking(StallBooking* booking);
    void removeBooking(StallBooking* booking);

    // Waitlist entries
    QVector<WaitlistEntry*>& getWaitlistEntries();
    void addWaitlistEntry(WaitlistEntry* entry);
    void removeWaitlistEntry(WaitlistEntry* entry);

    // Availability checks
    bool hasFoodStallAvailable() const;
    bool hasArtisanStallAvailable() const;

private:
    // Qt's built-in class to handle dates
    QDate m_date;
    int m_maxFoodStalls;
    int m_maxArtisanStalls;

    QVector<StallBooking*> m_bookings;
    QVector<WaitlistEntry*> m_waitlistEntries;
};

#endif // MARKETDATE_H