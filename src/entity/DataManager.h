#ifndef DATAMANAGER_H
#define DATAMANAGER_H

#include <QVector>
#include <QString>
#include <QDate>

class User;
class Vendor;
class MarketOperator;
class SystemAdmin;
class MarketDate;
class StallBooking;
class WaitlistEntry;

class DataManager {
public:
    static DataManager& instance();

    void initializeDefaultData();

    QVector<User*>& getUsers();
    User* findUserByUsername(const QString& username);
    QVector<Vendor*> getVendors();

    QVector<MarketDate*>& getMarketDates();
    MarketDate* findMarketDateByDate(const QDate& date);

    QVector<StallBooking*>& getBookings();
    void addBooking(StallBooking* booking);
    void removeBooking(StallBooking* booking);

    QVector<WaitlistEntry*>& getWaitlistEntries();
    void addWaitlistEntry(WaitlistEntry* entry);
    void removeWaitlistEntry(WaitlistEntry* entry);

private:
    DataManager();
    ~DataManager();
    DataManager(const DataManager&) = delete;
    DataManager& operator=(const DataManager&) = delete;

    QVector<User*> m_users;
    QVector<MarketDate*> m_marketDates;
    QVector<StallBooking*> m_bookings;
    QVector<WaitlistEntry*> m_waitlistEntries;
};

#endif // DATAMANAGER_H
