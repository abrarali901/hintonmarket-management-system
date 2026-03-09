#include "StallBooking.h"
#include "Vendor.h"
#include "MarketDate.h"

StallBooking::StallBooking()
    : m_vendor(nullptr), m_marketDate(nullptr), m_bookingTime(QDateTime::currentDateTime()) {
}

StallBooking::StallBooking(Vendor* vendor, MarketDate* marketDate)
    : m_vendor(vendor), m_marketDate(marketDate), m_bookingTime(QDateTime::currentDateTime()) {
}

StallBooking::~StallBooking() {
}

Vendor* StallBooking::getVendor() const { return m_vendor; }
MarketDate* StallBooking::getMarketDate() const { return m_marketDate; }
QDateTime StallBooking::getBookingTime() const { return m_bookingTime; }

void StallBooking::setVendor(Vendor* vendor) { m_vendor = vendor; }
void StallBooking::setMarketDate(MarketDate* marketDate) { m_marketDate = marketDate; }
