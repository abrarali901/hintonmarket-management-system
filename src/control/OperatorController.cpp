/**
 * @file OperatorController.cpp
 * @brief Market Operator business logic (D2 feature).
 *
 * Delegates to BookingController and WaitlistController so
 * the same business rules apply for operator actions.
 *
 * @author Victor
 */
#include "OperatorController.h"
#include "BookingController.h"
#include "WaitlistController.h"
#include "DataManager.h"
#include "Vendor.h"
#include "MarketDate.h"
#include "StallBooking.h"
#include "WaitlistEntry.h"

OperatorController::OperatorController()
    : m_bookingController(new BookingController()),
      m_waitlistController(new WaitlistController()) {
}

OperatorController::~OperatorController() {
    delete m_bookingController;
    delete m_waitlistController;
}

bool OperatorController::bookStallForVendor(Vendor* vendor, MarketDate* date) {
    if (!vendor || !date) return false;
    return m_bookingController->bookStall(vendor, date);
}

bool OperatorController::cancelBookingForVendor(StallBooking* booking) {
    if (!booking) return false;
    return m_bookingController->cancelBooking(booking);
}

bool OperatorController::removeVendorFromWaitlist(WaitlistEntry* entry) {
    if (!entry) return false;
    return m_waitlistController->leaveWaitlist(entry);
}

QVector<Vendor*> OperatorController::getAllVendors() {
    return DataManager::instance().getVendors();
}

QVector<MarketDate*> OperatorController::getAllMarketDates() {
    return DataManager::instance().getMarketDates();
}
