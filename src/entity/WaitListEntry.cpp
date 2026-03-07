#include "WaitlistEntry.h"
#include "Vendor.h"
#include "MarketDate.h"

WaitlistEntry::WaitlistEntry()
    : m_vendor(nullptr), m_marketDate(nullptr), m_joinTime(QDateTime::currentDateTime()), m_position(0) {
}

WaitlistEntry::WaitlistEntry(Vendor* vendor, MarketDate* marketDate)
    : m_vendor(vendor), m_marketDate(marketDate), m_joinTime(QDateTime::currentDateTime()), m_position(0) {
}

WaitlistEntry::~WaitlistEntry() {
}

Vendor* WaitlistEntry::getVendor() const {
    return m_vendor;
}

MarketDate* WaitlistEntry::getMarketDate() const {
    return m_marketDate;
}

QDateTime WaitlistEntry::getJoinTime() const {
    return m_joinTime;
}

int WaitlistEntry::getPosition() const {
    return m_position;
}

void WaitlistEntry::setVendor(Vendor* vendor) {
    m_vendor = vendor;
}

void WaitlistEntry::setMarketDate(MarketDate* marketDate) {
    m_marketDate = marketDate;
}

void WaitlistEntry::setPosition(int position) {
    m_position = position;
}