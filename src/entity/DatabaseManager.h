#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QString>
#include <QVector>

class User;
class Vendor;
class MarketOperator;
class SystemAdmin;
class MarketDate;
class StallBooking;
class WaitlistEntry;
class ComplianceDocument;
class Notification;

/**
 * @class DatabaseManager
 * @brief Singleton handling all SQLite database operations (D2).
 *
 * Implements the ORM layer: loads DB rows into objects and
 * persists changes (inserts, deletes, updates) back to SQLite.
 * The DB file (hintonMarket.sqlite3) is pre-populated with
 * 10 users, 4 market dates, and compliance docs.
 *
 * @author Victor, Osasuyi
 */
class DatabaseManager {
public:
    static DatabaseManager& instance();

    bool initialize();   // open DB connection
    bool isOpen() const;

    // Load all data from DB into memory objects
    QVector<User*> loadUsers();
    QVector<MarketDate*> loadMarketDates();
    QVector<StallBooking*> loadBookings(QVector<User*>& users, QVector<MarketDate*>& dates);
    QVector<WaitlistEntry*> loadWaitlistEntries(QVector<User*>& users, QVector<MarketDate*>& dates);
    void loadComplianceDocuments(QVector<User*>& users);
    void loadNotifications(QVector<User*>& users);

    // Booking persistence
    int insertBooking(int vendorId, int marketDateId);
    bool deleteBooking(int bookingId);

    // Waitlist persistence
    int insertWaitlistEntry(int vendorId, int marketDateId, int position);
    bool deleteWaitlistEntry(int waitlistId);
    bool updateWaitlistPosition(int waitlistId, int newPosition);

    // Notification persistence
    int insertNotification(int vendorId, int type, const QString& message);

    // (booked counts are derived from bookings table, no separate update needed)
    bool updateMarketDateBookedCounts(int marketDateId, int bookedFood, int bookedArtisan);

private:
    DatabaseManager();
    ~DatabaseManager();
    DatabaseManager(const DatabaseManager&) = delete;
    DatabaseManager& operator=(const DatabaseManager&) = delete;

    QSqlDatabase m_db;

    void createTables();
    Vendor* findVendorById(QVector<User*>& users, int id);
    MarketDate* findMarketDateById(QVector<MarketDate*>& dates, int id);
};

#endif // DATABASEMANAGER_H
