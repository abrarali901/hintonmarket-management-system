#include "DataManager.h"
#include "User.h"
#include "Vendor.h"
#include "MarketOperator.h"
#include "SystemAdmin.h"
#include "MarketDate.h"
#include "StallBooking.h"
#include "WaitlistEntry.h"
#include "ComplianceDocument.h"

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

void DataManager::initializeDefaultData() {
    // Clear any existing data
    qDeleteAll(m_users);
    m_users.clear();
    qDeleteAll(m_marketDates);
    m_marketDates.clear();
    qDeleteAll(m_bookings);
    m_bookings.clear();
    qDeleteAll(m_waitlistEntries);
    m_waitlistEntries.clear();

    QDate seasonEnd(QDate::currentDate().year(), 9, 30);

    // ========== 4 Food Vendors ==========
    Vendor* fv1 = new Vendor("Alice Johnson", "foodvendor1",
        "Alice's Fresh Produce", "alice@freshproduce.ca",
        "613-555-0101", "123 Elm St, Hintonville, ON K1A 0A1",
        Vendor::VendorCategory::FOOD);
    fv1->addComplianceDocument(new ComplianceDocument(
        ComplianceDocument::DocumentType::BUSINESS_LICENCE, "BL-2026-001", seasonEnd.addDays(30)));
    fv1->addComplianceDocument(new ComplianceDocument(
        ComplianceDocument::DocumentType::LIABILITY_INSURANCE, "LI-2026-001", seasonEnd.addDays(30)));
    ComplianceDocument* fv1Ins = fv1->getComplianceDocuments().last();
    fv1Ins->setInsuranceProvider("Ontario Shield Insurance");
    fv1->addComplianceDocument(new ComplianceDocument(
        ComplianceDocument::DocumentType::FOOD_HANDLER_CERTIFICATION, "FH-2026-001", seasonEnd.addDays(30)));

    Vendor* fv2 = new Vendor("Bob Martinez", "foodvendor2",
        "Bob's Bakery", "bob@bakery.ca",
        "613-555-0102", "456 Oak Ave, Hintonville, ON K1A 0A2",
        Vendor::VendorCategory::FOOD);
    fv2->addComplianceDocument(new ComplianceDocument(
        ComplianceDocument::DocumentType::BUSINESS_LICENCE, "BL-2026-002", seasonEnd.addDays(30)));
    fv2->addComplianceDocument(new ComplianceDocument(
        ComplianceDocument::DocumentType::LIABILITY_INSURANCE, "LI-2026-002", seasonEnd.addDays(30)));
    ComplianceDocument* fv2Ins = fv2->getComplianceDocuments().last();
    fv2Ins->setInsuranceProvider("MapleGuard Insurance");
    fv2->addComplianceDocument(new ComplianceDocument(
        ComplianceDocument::DocumentType::FOOD_HANDLER_CERTIFICATION, "FH-2026-002", seasonEnd.addDays(30)));

    Vendor* fv3 = new Vendor("Clara Nguyen", "foodvendor3",
        "Clara's Jams & Preserves", "clara@jams.ca",
        "613-555-0103", "789 Maple Rd, Hintonville, ON K1A 0A3",
        Vendor::VendorCategory::FOOD);
    fv3->addComplianceDocument(new ComplianceDocument(
        ComplianceDocument::DocumentType::BUSINESS_LICENCE, "BL-2026-003", seasonEnd.addDays(30)));
    fv3->addComplianceDocument(new ComplianceDocument(
        ComplianceDocument::DocumentType::LIABILITY_INSURANCE, "LI-2026-003", seasonEnd.addDays(30)));
    ComplianceDocument* fv3Ins = fv3->getComplianceDocuments().last();
    fv3Ins->setInsuranceProvider("TrueNorth Coverage");
    fv3->addComplianceDocument(new ComplianceDocument(
        ComplianceDocument::DocumentType::FOOD_HANDLER_CERTIFICATION, "FH-2026-003", seasonEnd.addDays(30)));

    Vendor* fv4 = new Vendor("David Park", "foodvendor4",
        "David's Organic Juices", "david@juices.ca",
        "613-555-0104", "321 Birch Ln, Hintonville, ON K1A 0A4",
        Vendor::VendorCategory::FOOD);
    fv4->addComplianceDocument(new ComplianceDocument(
        ComplianceDocument::DocumentType::BUSINESS_LICENCE, "BL-2026-004", seasonEnd.addDays(30)));
    fv4->addComplianceDocument(new ComplianceDocument(
        ComplianceDocument::DocumentType::LIABILITY_INSURANCE, "LI-2026-004", seasonEnd.addDays(30)));
    ComplianceDocument* fv4Ins = fv4->getComplianceDocuments().last();
    fv4Ins->setInsuranceProvider("Ontario Shield Insurance");
    fv4->addComplianceDocument(new ComplianceDocument(
        ComplianceDocument::DocumentType::FOOD_HANDLER_CERTIFICATION, "FH-2026-004", seasonEnd.addDays(30)));

    // ========== 4 Artisan Vendors ==========
    Vendor* av1 = new Vendor("Emma Wilson", "artisanvendor1",
        "Emma's Handmade Pottery", "emma@pottery.ca",
        "613-555-0201", "100 Pine St, Hintonville, ON K1A 0B1",
        Vendor::VendorCategory::ARTISAN);
    av1->addComplianceDocument(new ComplianceDocument(
        ComplianceDocument::DocumentType::BUSINESS_LICENCE, "BL-2026-005", seasonEnd.addDays(30)));
    av1->addComplianceDocument(new ComplianceDocument(
        ComplianceDocument::DocumentType::LIABILITY_INSURANCE, "LI-2026-005", seasonEnd.addDays(30)));
    ComplianceDocument* av1Ins = av1->getComplianceDocuments().last();
    av1Ins->setInsuranceProvider("ArtisanProtect Insurance");

    Vendor* av2 = new Vendor("Frank Chen", "artisanvendor2",
        "Frank's Woodcraft", "frank@woodcraft.ca",
        "613-555-0202", "200 Cedar Dr, Hintonville, ON K1A 0B2",
        Vendor::VendorCategory::ARTISAN);
    av2->addComplianceDocument(new ComplianceDocument(
        ComplianceDocument::DocumentType::BUSINESS_LICENCE, "BL-2026-006", seasonEnd.addDays(30)));
    av2->addComplianceDocument(new ComplianceDocument(
        ComplianceDocument::DocumentType::LIABILITY_INSURANCE, "LI-2026-006", seasonEnd.addDays(30)));
    ComplianceDocument* av2Ins = av2->getComplianceDocuments().last();
    av2Ins->setInsuranceProvider("MapleGuard Insurance");

    Vendor* av3 = new Vendor("Grace Lee", "artisanvendor3",
        "Grace's Jewelry Studio", "grace@jewelry.ca",
        "613-555-0203", "300 Spruce Ct, Hintonville, ON K1A 0B3",
        Vendor::VendorCategory::ARTISAN);
    av3->addComplianceDocument(new ComplianceDocument(
        ComplianceDocument::DocumentType::BUSINESS_LICENCE, "BL-2026-007", seasonEnd.addDays(30)));
    av3->addComplianceDocument(new ComplianceDocument(
        ComplianceDocument::DocumentType::LIABILITY_INSURANCE, "LI-2026-007", seasonEnd.addDays(30)));
    ComplianceDocument* av3Ins = av3->getComplianceDocuments().last();
    av3Ins->setInsuranceProvider("TrueNorth Coverage");

    Vendor* av4 = new Vendor("Henry Adams", "artisanvendor4",
        "Henry's Candles & Soaps", "henry@candles.ca",
        "613-555-0204", "400 Willow Way, Hintonville, ON K1A 0B4",
        Vendor::VendorCategory::ARTISAN);
    av4->addComplianceDocument(new ComplianceDocument(
        ComplianceDocument::DocumentType::BUSINESS_LICENCE, "BL-2026-008", seasonEnd.addDays(30)));
    av4->addComplianceDocument(new ComplianceDocument(
        ComplianceDocument::DocumentType::LIABILITY_INSURANCE, "LI-2026-008", seasonEnd.addDays(30)));
    ComplianceDocument* av4Ins = av4->getComplianceDocuments().last();
    av4Ins->setInsuranceProvider("ArtisanProtect Insurance");

    // ========== 1 Market Operator ==========
    MarketOperator* op1 = new MarketOperator("Irene Thompson", "operator1");

    // ========== 1 System Administrator ==========
    SystemAdmin* admin1 = new SystemAdmin("James Rivera", "admin1");

    // Add all users
    m_users.append(fv1);
    m_users.append(fv2);
    m_users.append(fv3);
    m_users.append(fv4);
    m_users.append(av1);
    m_users.append(av2);
    m_users.append(av3);
    m_users.append(av4);
    m_users.append(op1);
    m_users.append(admin1);

    // ========== 4 Market Weeks (Sundays) ==========
    // Find next 4 Sundays from today
    QDate today = QDate::currentDate();
    QDate nextSunday = today;
    // Advance to next Sunday (dayOfWeek: 1=Mon, 7=Sun)
    int daysUntilSunday = (7 - today.dayOfWeek()) % 7;
    if (daysUntilSunday == 0) daysUntilSunday = 7; // skip today if it's Sunday
    nextSunday = today.addDays(daysUntilSunday);

    for (int i = 0; i < 4; i++) {
        QDate marketDay = nextSunday.addDays(i * 7);
        m_marketDates.append(new MarketDate(marketDay));
    }
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
