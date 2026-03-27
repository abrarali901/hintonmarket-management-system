/**
 * @file DatabaseManager.cpp
 * @brief SQLite persistence layer - all CRUD operations.
 *
 * Loads DB rows into objects on startup, and persists
 * bookings/waitlist/notification changes in real time.
 *
 * @author Victor, Osasuyi
 */
#include "DatabaseManager.h"
#include "User.h"
#include "Vendor.h"
#include "MarketOperator.h"
#include "SystemAdmin.h"
#include "MarketDate.h"
#include "StallBooking.h"
#include "WaitlistEntry.h"
#include "ComplianceDocument.h"
#include "Notification.h"
#include <QCoreApplication>
#include <QDebug>
#include <QDateTime>

DatabaseManager::DatabaseManager() {
}

DatabaseManager::~DatabaseManager() {
    if (m_db.isOpen()) {
        m_db.close();
    }
}

DatabaseManager& DatabaseManager::instance() {
    static DatabaseManager dm;
    return dm;
}

bool DatabaseManager::initialize() {
    m_db = QSqlDatabase::addDatabase("QSQLITE");
    QString dbPath = QCoreApplication::applicationDirPath() + "/hintonMarket.sqlite3";
    qDebug() << "Looking for DB at:" << dbPath;
    m_db.setDatabaseName(dbPath);

    if (!m_db.open()) {
        qDebug() << "Database Error:" << m_db.lastError().text();
        return false;
    }
    qDebug() << "Database connected successfully.";

    // Enable foreign keys
    QSqlQuery pragma(m_db);
    pragma.exec("PRAGMA foreign_keys = ON;");

    return true;
}

bool DatabaseManager::isOpen() const {
    return m_db.isOpen();
}

void DatabaseManager::createTables() {
    QSqlQuery query(m_db);

    query.exec("CREATE TABLE IF NOT EXISTS users ("
               "id INTEGER PRIMARY KEY AUTOINCREMENT, "
               "name TEXT NOT NULL, "
               "username TEXT NOT NULL UNIQUE, "
               "user_type INTEGER NOT NULL, "
               "business_name TEXT, "
               "email TEXT, "
               "phone TEXT, "
               "address TEXT, "
               "vendor_category INTEGER)");

    query.exec("CREATE TABLE IF NOT EXISTS market_dates ("
               "id INTEGER PRIMARY KEY AUTOINCREMENT, "
               "date TEXT NOT NULL UNIQUE, "
               "max_food_stalls INTEGER DEFAULT 2, "
               "max_artisan_stalls INTEGER DEFAULT 2)");

    query.exec("CREATE TABLE IF NOT EXISTS compliance_documents ("
               "id INTEGER PRIMARY KEY AUTOINCREMENT, "
               "vendor_id INTEGER NOT NULL, "
               "doc_type INTEGER NOT NULL, "
               "document_number TEXT NOT NULL, "
               "expiration_date TEXT NOT NULL, "
               "insurance_provider TEXT, "
               "FOREIGN KEY (vendor_id) REFERENCES users(id))");

    query.exec("CREATE TABLE IF NOT EXISTS bookings ("
               "id INTEGER PRIMARY KEY AUTOINCREMENT, "
               "vendor_id INTEGER NOT NULL, "
               "market_date_id INTEGER NOT NULL, "
               "booking_time TEXT NOT NULL, "
               "FOREIGN KEY (vendor_id) REFERENCES users(id), "
               "FOREIGN KEY (market_date_id) REFERENCES market_dates(id))");

    query.exec("CREATE TABLE IF NOT EXISTS waitlist_entries ("
               "id INTEGER PRIMARY KEY AUTOINCREMENT, "
               "vendor_id INTEGER NOT NULL, "
               "market_date_id INTEGER NOT NULL, "
               "join_time TEXT NOT NULL, "
               "position INTEGER NOT NULL, "
               "FOREIGN KEY (vendor_id) REFERENCES users(id), "
               "FOREIGN KEY (market_date_id) REFERENCES market_dates(id))");

    query.exec("CREATE TABLE IF NOT EXISTS notifications ("
               "id INTEGER PRIMARY KEY AUTOINCREMENT, "
               "vendor_id INTEGER NOT NULL, "
               "type INTEGER NOT NULL, "
               "message TEXT NOT NULL, "
               "timestamp TEXT NOT NULL, "
               "is_read INTEGER DEFAULT 0, "
               "FOREIGN KEY (vendor_id) REFERENCES users(id))");
}

// ==================== LOAD OPERATIONS ====================

QVector<User*> DatabaseManager::loadUsers() {
    QVector<User*> users;
    QSqlQuery query(m_db);
    query.exec("SELECT id, name, username, user_type, business_name, email, phone, address, vendor_category FROM users");

    while (query.next()) {
        int id = query.value(0).toInt();
        QString name = query.value(1).toString();
        QString username = query.value(2).toString();
        int userType = query.value(3).toInt();

        User* user = nullptr;

        if (userType == 0) {
            // Vendor
            QString businessName = query.value(4).toString();
            QString email = query.value(5).toString();
            QString phone = query.value(6).toString();
            QString address = query.value(7).toString();
            int category = query.value(8).toInt();

            Vendor* vendor = new Vendor(name, username, businessName, email, phone, address,
                static_cast<Vendor::VendorCategory>(category));
            vendor->setId(id);
            user = vendor;
        } else if (userType == 1) {
            MarketOperator* op = new MarketOperator(name, username);
            op->setId(id);
            user = op;
        } else if (userType == 2) {
            SystemAdmin* admin = new SystemAdmin(name, username);
            admin->setId(id);
            user = admin;
        }

        if (user) {
            users.append(user);
        }
    }

    return users;
}

QVector<MarketDate*> DatabaseManager::loadMarketDates() {
    QVector<MarketDate*> dates;
    QSqlQuery query(m_db);
    query.exec("SELECT id, date, max_food_stalls, max_artisan_stalls FROM market_dates ORDER BY date");

    while (query.next()) {
        int id = query.value(0).toInt();
        QDate date = QDate::fromString(query.value(1).toString(), "yyyy-MM-dd");

        MarketDate* md = new MarketDate(date);
        md->setId(id);
        dates.append(md);
    }

    return dates;
}

void DatabaseManager::loadComplianceDocuments(QVector<User*>& users) {
    QSqlQuery query(m_db);
    query.exec("SELECT id, vendor_id, doc_type, document_number, expiration_date, insurance_provider "
               "FROM compliance_documents");

    while (query.next()) {
        int docId = query.value(0).toInt();
        int vendorId = query.value(1).toInt();
        int docType = query.value(2).toInt();
        QString docNumber = query.value(3).toString();
        QDate expDate = QDate::fromString(query.value(4).toString(), "yyyy-MM-dd");
        QString provider = query.value(5).toString();

        Vendor* vendor = findVendorById(users, vendorId);
        if (vendor) {
            ComplianceDocument* doc = new ComplianceDocument(
                static_cast<ComplianceDocument::DocumentType>(docType), docNumber, expDate);
            doc->setId(docId);
            if (!provider.isEmpty()) {
                doc->setInsuranceProvider(provider);
            }
            vendor->addComplianceDocument(doc);
        }
    }
}

QVector<StallBooking*> DatabaseManager::loadBookings(QVector<User*>& users, QVector<MarketDate*>& dates) {
    QVector<StallBooking*> bookings;
    QSqlQuery query(m_db);
    query.exec("SELECT id, vendor_id, market_date_id, booking_time FROM bookings");

    while (query.next()) {
        int bookingId = query.value(0).toInt();
        int vendorId = query.value(1).toInt();
        int dateId = query.value(2).toInt();
        QDateTime bookingTime = QDateTime::fromString(query.value(3).toString(), Qt::ISODate);

        Vendor* vendor = findVendorById(users, vendorId);
        MarketDate* marketDate = findMarketDateById(dates, dateId);

        if (vendor && marketDate) {
            StallBooking* booking = new StallBooking(vendor, marketDate);
            booking->setId(bookingId);
            booking->setBookingTime(bookingTime);

            // Wire up relationships
            vendor->addBooking(booking);
            marketDate->addBooking(booking);
            bookings.append(booking);
        }
    }

    return bookings;
}

QVector<WaitlistEntry*> DatabaseManager::loadWaitlistEntries(QVector<User*>& users, QVector<MarketDate*>& dates) {
    QVector<WaitlistEntry*> entries;
    QSqlQuery query(m_db);
    query.exec("SELECT id, vendor_id, market_date_id, join_time, position FROM waitlist_entries "
               "ORDER BY position ASC");

    while (query.next()) {
        int entryId = query.value(0).toInt();
        int vendorId = query.value(1).toInt();
        int dateId = query.value(2).toInt();
        QDateTime joinTime = QDateTime::fromString(query.value(3).toString(), Qt::ISODate);
        int position = query.value(4).toInt();

        Vendor* vendor = findVendorById(users, vendorId);
        MarketDate* marketDate = findMarketDateById(dates, dateId);

        if (vendor && marketDate) {
            WaitlistEntry* entry = new WaitlistEntry(vendor, marketDate);
            entry->setId(entryId);
            entry->setJoinTime(joinTime);
            entry->setPosition(position);

            vendor->addWaitlistEntry(entry);
            marketDate->addWaitlistEntry(entry);
            entries.append(entry);
        }
    }

    return entries;
}

void DatabaseManager::loadNotifications(QVector<User*>& users) {
    QSqlQuery query(m_db);
    query.exec("SELECT id, vendor_id, type, message, timestamp, is_read FROM notifications "
               "ORDER BY timestamp ASC");

    while (query.next()) {
        int notifId = query.value(0).toInt();
        int vendorId = query.value(1).toInt();
        int type = query.value(2).toInt();
        QString message = query.value(3).toString();
        QDateTime timestamp = QDateTime::fromString(query.value(4).toString(), Qt::ISODate);
        bool isRead = query.value(5).toBool();

        Vendor* vendor = findVendorById(users, vendorId);
        if (vendor) {
            Notification* notif = new Notification(
                static_cast<Notification::NotificationType>(type), message);
            notif->setId(notifId);
            notif->setTimestamp(timestamp);
            if (isRead) notif->markAsRead();
            vendor->addNotification(notif);
        }
    }
}

// ==================== INSERT OPERATIONS ====================

int DatabaseManager::insertBooking(int vendorId, int marketDateId) {
    QSqlQuery query(m_db);
    query.prepare("INSERT INTO bookings (vendor_id, market_date_id, booking_time) "
                  "VALUES (:vendor_id, :market_date_id, :booking_time)");
    query.bindValue(":vendor_id", vendorId);
    query.bindValue(":market_date_id", marketDateId);
    query.bindValue(":booking_time", QDateTime::currentDateTime().toString(Qt::ISODate));

    if (query.exec()) {
        return query.lastInsertId().toInt();
    }
    qDebug() << "Insert booking error:" << query.lastError().text();
    return -1;
}

bool DatabaseManager::deleteBooking(int bookingId) {
    QSqlQuery query(m_db);
    query.prepare("DELETE FROM bookings WHERE id = :id");
    query.bindValue(":id", bookingId);

    if (!query.exec()) {
        qDebug() << "Delete booking error:" << query.lastError().text();
        return false;
    }
    return true;
}

int DatabaseManager::insertWaitlistEntry(int vendorId, int marketDateId, int position) {
    QSqlQuery query(m_db);
    query.prepare("INSERT INTO waitlist_entries (vendor_id, market_date_id, join_time, position) "
                  "VALUES (:vendor_id, :market_date_id, :join_time, :position)");
    query.bindValue(":vendor_id", vendorId);
    query.bindValue(":market_date_id", marketDateId);
    query.bindValue(":join_time", QDateTime::currentDateTime().toString(Qt::ISODate));
    query.bindValue(":position", position);

    if (query.exec()) {
        return query.lastInsertId().toInt();
    }
    qDebug() << "Insert waitlist error:" << query.lastError().text();
    return -1;
}

bool DatabaseManager::deleteWaitlistEntry(int waitlistId) {
    QSqlQuery query(m_db);
    query.prepare("DELETE FROM waitlist_entries WHERE id = :id");
    query.bindValue(":id", waitlistId);

    if (!query.exec()) {
        qDebug() << "Delete waitlist error:" << query.lastError().text();
        return false;
    }
    return true;
}

bool DatabaseManager::updateWaitlistPosition(int waitlistId, int newPosition) {
    QSqlQuery query(m_db);
    query.prepare("UPDATE waitlist_entries SET position = :position WHERE id = :id");
    query.bindValue(":position", newPosition);
    query.bindValue(":id", waitlistId);

    if (!query.exec()) {
        qDebug() << "Update waitlist position error:" << query.lastError().text();
        return false;
    }
    return true;
}

int DatabaseManager::insertNotification(int vendorId, int type, const QString& message) {
    QSqlQuery query(m_db);
    query.prepare("INSERT INTO notifications (vendor_id, type, message, timestamp, is_read) "
                  "VALUES (:vendor_id, :type, :message, :timestamp, 0)");
    query.bindValue(":vendor_id", vendorId);
    query.bindValue(":type", type);
    query.bindValue(":message", message);
    query.bindValue(":timestamp", QDateTime::currentDateTime().toString(Qt::ISODate));

    if (query.exec()) {
        return query.lastInsertId().toInt();
    }
    qDebug() << "Insert notification error:" << query.lastError().text();
    return -1;
}

bool DatabaseManager::updateMarketDateBookedCounts(int marketDateId, int bookedFood, int bookedArtisan) {
    Q_UNUSED(marketDateId)
    Q_UNUSED(bookedFood)
    Q_UNUSED(bookedArtisan)
    // Booked counts are derived from bookings table at load time, no separate update needed
    return true;
}

// ==================== HELPER METHODS ====================

Vendor* DatabaseManager::findVendorById(QVector<User*>& users, int id) {
    for (User* user : users) {
        if (user->getId() == id && user->getUserType() == User::UserType::VENDOR) {
            return static_cast<Vendor*>(user);
        }
    }
    return nullptr;
}

MarketDate* DatabaseManager::findMarketDateById(QVector<MarketDate*>& dates, int id) {
    for (MarketDate* md : dates) {
        if (md->getId() == id) {
            return md;
        }
    }
    return nullptr;
}
