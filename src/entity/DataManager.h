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

/**
 * @class DataManager
 * @brief Singleton that holds all in-memory data objects.
 *
 * Acts as the central repository (Repository pattern). All controllers
 * go through DataManager to access users, market dates, bookings, etc.
 * On startup, calls DatabaseManager to load everything from SQLite.
 *
 * Changed in D2: replaced initializeDefaultData() with loadFromDatabase()
 * so data persists between runs.
 *
 * @author Sheng, Victor
 */
class DataManager {
public:
    static DataManager& instance();

    // D2: loads all objects from the SQLite database
    void loadFromDatabase();

    // User access
    QVector<User*>& getUsers();
    User* findUserByUsername(const QString& username);
    QVector<Vendor*> getVendors();

    // Market date access
    QVector<MarketDate*>& getMarketDates();
    MarketDate* findMarketDateByDate(const QDate& date);

    // Booking management
    QVector<StallBooking*>& getBookings();
    void addBooking(StallBooking* booking);
    void removeBooking(StallBooking* booking);

    // Waitlist management
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
