#ifndef MARKETDATE_H
#define MARKETDATE_H

#include <QDate>
#include <QVector>

class StallBooking;
class WaitlistEntry;

class MarketDate {
public:
    MarketDate();
    MarketDate(const QDate& date);
    ~MarketDate();

    QDate getDate() const;
    QString getDateString() const;
    int getMaxFoodStalls() const;
    int getMaxArtisanStalls() const;
    int getBookedFoodStalls() const;
    int getBookedArtisanStalls() const;
    int getAvailableFoodStalls() const;
    int getAvailableArtisanStalls() const;

    void setDate(const QDate& date);

    QVector<StallBooking*>& getBookings();
    void addBooking(StallBooking* booking);
    void removeBooking(StallBooking* booking);

    QVector<WaitlistEntry*>& getWaitlistEntries();
    void addWaitlistEntry(WaitlistEntry* entry);
    void removeWaitlistEntry(WaitlistEntry* entry);

    bool hasFoodStallAvailable() const;
    bool hasArtisanStallAvailable() const;

private:
    QDate m_date;
    int m_maxFoodStalls;
    int m_maxArtisanStalls;
    QVector<StallBooking*> m_bookings;
    QVector<WaitlistEntry*> m_waitlistEntries;
};

#endif // MARKETDATE_H
