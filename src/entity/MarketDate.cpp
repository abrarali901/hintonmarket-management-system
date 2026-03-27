// MarketDate implementation (Victor)
#include "MarketDate.h"
#include "StallBooking.h"
#include "WaitlistEntry.h"
#include "Vendor.h"

MarketDate::MarketDate()
    : m_id(0), m_date(QDate()), m_maxFoodStalls(2), m_maxArtisanStalls(2) {
}

MarketDate::MarketDate(const QDate& date)
    : m_id(0), m_date(date), m_maxFoodStalls(2), m_maxArtisanStalls(2) {
}

MarketDate::~MarketDate() {
}

int MarketDate::getId() const { return m_id; }
QDate MarketDate::getDate() const { return m_date; }

QString MarketDate::getDateString() const {
    return m_date.toString("yyyy-MM-dd");
}

int MarketDate::getMaxFoodStalls() const { return m_maxFoodStalls; }
int MarketDate::getMaxArtisanStalls() const { return m_maxArtisanStalls; }

int MarketDate::getBookedFoodStalls() const {
    int count = 0;
    for (StallBooking* booking : m_bookings) {
        if (booking->getVendor()->getCategory() == Vendor::VendorCategory::FOOD) {
            count++;
        }
    }
    return count;
}

int MarketDate::getBookedArtisanStalls() const {
    int count = 0;
    for (StallBooking* booking : m_bookings) {
        if (booking->getVendor()->getCategory() == Vendor::VendorCategory::ARTISAN) {
            count++;
        }
    }
    return count;
}

int MarketDate::getAvailableFoodStalls() const {
    return m_maxFoodStalls - getBookedFoodStalls();
}

int MarketDate::getAvailableArtisanStalls() const {
    return m_maxArtisanStalls - getBookedArtisanStalls();
}

void MarketDate::setId(int id) { m_id = id; }
void MarketDate::setDate(const QDate& date) { m_date = date; }

QVector<StallBooking*>& MarketDate::getBookings() { return m_bookings; }
void MarketDate::addBooking(StallBooking* booking) { m_bookings.append(booking); }
void MarketDate::removeBooking(StallBooking* booking) { m_bookings.removeOne(booking); }

QVector<WaitlistEntry*>& MarketDate::getWaitlistEntries() { return m_waitlistEntries; }
void MarketDate::addWaitlistEntry(WaitlistEntry* entry) { m_waitlistEntries.append(entry); }
void MarketDate::removeWaitlistEntry(WaitlistEntry* entry) { m_waitlistEntries.removeOne(entry); }

bool MarketDate::hasFoodStallAvailable() const {
    return getAvailableFoodStalls() > 0;
}

bool MarketDate::hasArtisanStallAvailable() const {
    return getAvailableArtisanStalls() > 0;
}
