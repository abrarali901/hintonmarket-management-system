#include "BookingController.h"
#include "DataManager.h"
#include "Vendor.h"
#include "MarketDate.h"
#include "StallBooking.h"
#include "Notification.h"
#include "ComplianceDocument.h"

BookingController::BookingController() {
}

BookingController::~BookingController() {
}

bool BookingController::bookStall(Vendor* vendor, MarketDate* date) {
    if (!vendor || !date) return false;

    // Check if vendor can book
    if (!canVendorBook(vendor)) return false;

    // Check availability for vendor's category
    if (vendor->getCategory() == Vendor::VendorCategory::FOOD) {
        if (!date->hasFoodStallAvailable()) return false;
    } else {
        if (!date->hasArtisanStallAvailable()) return false;
    }

    // Create booking
    StallBooking* booking = new StallBooking(vendor, date);

    // Add to DataManager
    DataManager::instance().addBooking(booking);

    // Add to MarketDate
    date->addBooking(booking);

    // Add to Vendor
    vendor->addBooking(booking);

    // Notify vendor
    notifyBookingConfirmed(vendor, date);

    return true;
}

bool BookingController::cancelBooking(StallBooking* booking) {
    if (!booking) return false;

    Vendor* vendor = booking->getVendor();
    MarketDate* date = booking->getMarketDate();

    // Remove from DataManager
    DataManager::instance().removeBooking(booking);

    // Remove from MarketDate
    date->removeBooking(booking);

    // Remove from Vendor
    vendor->removeBooking(booking);

    // Notify vendor
    notifyBookingCancelled(vendor, date);

    // TODO: Trigger waitlist check (WaitlistController)

    delete booking;
    return true;
}

bool BookingController::canVendorBook(Vendor* vendor) {
    if (!vendor) return false;

    // TODO: Check if vendor has all required compliance documents
    // Food vendors need 3, Artisan vendors need 2

    return true;
}

void BookingController::notifyBookingConfirmed(Vendor* vendor, MarketDate* date) {
    QString message = "Booking confirmed for " + date->getDateString();
    Notification* notification = new Notification(
        Notification::NotificationType::BOOKING_CONFIRMED, message);
    vendor->addNotification(notification);
}

void BookingController::notifyBookingCancelled(Vendor* vendor, MarketDate* date) {
    QString message = "Booking cancelled for " + date->getDateString();
    Notification* notification = new Notification(
        Notification::NotificationType::BOOKING_CANCELLED, message);
    vendor->addNotification(notification);
}