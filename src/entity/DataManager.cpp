/**
 * @file DataManager.cpp
 * @brief Central in-memory data repository (singleton).
 *
 * D2 change: now loads from SQLite instead of hardcoded data.
 *
 * @author Sheng, Victor
 */
#include "DataManager.h"
#include "DatabaseManager.h"
#include "User.h"
#include "Vendor.h"
#include "MarketOperator.h"
#include "SystemAdmin.h"
#include "MarketDate.h"
#include "StallBooking.h"
#include "WaitlistEntry.h"
#include "ComplianceDocument.h"
#include <QDebug>

DataManager::DataManager() {
}

DataManager::~DataManager() {
    qDeleteAll(m_users);
    qDeleteAll(m_marketDates);
    qDeleteAll(m_bookings);
    qDeleteAll(m_waitlistEntries);
}

DataManager& DataManager::instance() {
    static DataManager dm;
    return dm;
}

void DataManager::loadFromDatabase() {
    // Clear any existing data
    qDeleteAll(m_users);
    m_users.clear();
    qDeleteAll(m_marketDates);
    m_marketDates.clear();
    // Don't delete bookings/waitlist entries - they're owned by users/dates
    m_bookings.clear();
    m_waitlistEntries.clear();

    DatabaseManager& dbMgr = DatabaseManager::instance();

    // Load users (vendors, operators, admins)
    m_users = dbMgr.loadUsers();
    qDebug() << "Loaded" << m_users.size() << "users from database";

    // Load compliance documents for vendors
    dbMgr.loadComplianceDocuments(m_users);

    // Load market dates
    m_marketDates = dbMgr.loadMarketDates();
    qDebug() << "Loaded" << m_marketDates.size() << "market dates from database";

    // Load bookings (wires up vendor->booking and marketDate->booking)
    m_bookings = dbMgr.loadBookings(m_users, m_marketDates);
    qDebug() << "Loaded" << m_bookings.size() << "bookings from database";

    // Load waitlist entries (wires up vendor->entry and marketDate->entry)
    m_waitlistEntries = dbMgr.loadWaitlistEntries(m_users, m_marketDates);
    qDebug() << "Loaded" << m_waitlistEntries.size() << "waitlist entries from database";

    // Load notifications for vendors
    dbMgr.loadNotifications(m_users);
}

QVector<User*>& DataManager::getUsers() {
    return m_users;
}

User* DataManager::findUserByUsername(const QString& username) {
    for (User* user : m_users) {
        if (user->getUsername().compare(username, Qt::CaseInsensitive) == 0) {
            return user;
        }
    }
    return nullptr;
}

QVector<Vendor*> DataManager::getVendors() {
    QVector<Vendor*> vendors;
    for (User* user : m_users) {
        if (user->getUserType() == User::UserType::VENDOR) {
            vendors.append(static_cast<Vendor*>(user));
        }
    }
    return vendors;
}

QVector<MarketDate*>& DataManager::getMarketDates() {
    return m_marketDates;
}

MarketDate* DataManager::findMarketDateByDate(const QDate& date) {
    for (MarketDate* md : m_marketDates) {
        if (md->getDate() == date) {
            return md;
        }
    }
    return nullptr;
}

QVector<StallBooking*>& DataManager::getBookings() {
    return m_bookings;
}

void DataManager::addBooking(StallBooking* booking) {
    m_bookings.append(booking);
}

void DataManager::removeBooking(StallBooking* booking) {
    m_bookings.removeOne(booking);
}

QVector<WaitlistEntry*>& DataManager::getWaitlistEntries() {
    return m_waitlistEntries;
}

void DataManager::addWaitlistEntry(WaitlistEntry* entry) {
    m_waitlistEntries.append(entry);
}

void DataManager::removeWaitlistEntry(WaitlistEntry* entry) {
    m_waitlistEntries.removeOne(entry);
}
