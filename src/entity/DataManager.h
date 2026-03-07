#ifndef DATAMANAGER_H
#define DATAMANAGER_H

#include <QVector>
#include <QString>

class User;
class Vendor;
class MarketOperator;
class SystemAdmin;
class MarketDate;
class StallBooking;
class WaitlistEntry;

class DataManager {
public:
    // Singleton - only one instance
    static DataManager& instance();

    // Initialize default data (10 users, 4 market weeks)
    void initializeDefaultData();

    // User access
    QVector<User*>& getUsers();
    User* findUserByUsername(const QString& username);
    QVector<Vendor*> getVendors();

    // Market date access
    QVector<MarketDate*>& getMarketDates();
    MarketDate* findMarketDateByDate(const QDate& date);

    // Booking access
    QVector<StallBooking*>& getBookings();
    void addBooking(StallBooking* booking);
    void removeBooking(StallBooking* booking);

    // Waitlist access
    QVector<WaitlistEntry*>& getWaitlistEntries();
    void addWaitlistEntry(WaitlistEntry* entry);
    void removeWaitlistEntry(WaitlistEntry* entry);

private:
    // Private constructor - use instance() instead
    DataManager();
    ~DataManager();

    // Prevent copying
    DataManager(const DataManager&) = delete;
    DataManager& operator=(const DataManager&) = delete;

    // Data collections
    QVector<User*> m_users;
    QVector<MarketDate*> m_marketDates;
    QVector<StallBooking*> m_bookings;
    QVector<WaitlistEntry*> m_waitlistEntries;
};

#endif // DATAMANAGER_H