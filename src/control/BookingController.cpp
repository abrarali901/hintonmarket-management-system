#include "BookingController.h"
#include "DataManager.h"
#include "Vendor.h"
#include "MarketDate.h"
#include "StallBooking.h"
#include "Notification.h"
#include "ComplianceDocument.h"
#include "WaitlistEntry.h"

BookingController::BookingController() {
}

BookingController::~BookingController() {
}

bool BookingController::bookStall(Vendor* vendor, MarketDate* date) {
    if (!vendor || !date) return false;

    // Check compliance documents
    if (!canVendorBook(vendor)) return false;

    // Business rule: vendors can book only one stall at a time
    if (!vendor->getBookings().isEmpty()) return false;

    // Check availability for vendor's category
    if (vendor->getCategory() == Vendor::VendorCategory::FOOD) {
        if (!date->hasFoodStallAvailable()) return false;
    } else {
        if (!date->hasArtisanStallAvailable()) return false;
    }

    // Check vendor doesn't already have booking for this date
    for (StallBooking* existing : vendor->getBookings()) {
        if (existing->getMarketDate() == date) return false;
    }

    // Create booking
    StallBooking* booking = new StallBooking(vendor, date);

    // Add to DataManager, MarketDate, and Vendor
    DataManager::instance().addBooking(booking);
    date->addBooking(booking);
    vendor->addBooking(booking);

    // Notify vendor
    notifyBookingConfirmed(vendor, date);

    return true;
}

bool BookingController::cancelBooking(StallBooking* booking) {
    if (!booking) return false;

    Vendor* vendor = booking->getVendor();
    MarketDate* date = booking->getMarketDate();
    Vendor::VendorCategory category = vendor->getCategory();

    // Remove from DataManager, MarketDate, and Vendor
    DataManager::instance().removeBooking(booking);
    date->removeBooking(booking);
    vendor->removeBooking(booking);

    // Notify vendor of cancellation
    notifyBookingCancelled(vendor, date);

    // Trigger waitlist: notify first vendor in queue for this date/category
    QVector<WaitlistEntry*>& allEntries = date->getWaitlistEntries();
    WaitlistEntry* firstInQueue = nullptr;
    for (WaitlistEntry* entry : allEntries) {
        if (entry->getVendor()->getCategory() == category) {
            if (!firstInQueue || entry->getPosition() < firstInQueue->getPosition()) {
                firstInQueue = entry;
            }
        }
    }

    if (firstInQueue) {
        Vendor* waitVendor = firstInQueue->getVendor();
        QString message = "A stall is now available for " + date->getDateString() +
                         "! You are first in the waitlist queue.";
        Notification* notification = new Notification(
            Notification::NotificationType::WAITLIST_AVAILABLE, message);
        waitVendor->addNotification(notification);
    }

    delete booking;
    return true;
}

bool BookingController::canVendorBook(Vendor* vendor) {
    if (!vendor) return false;
    return vendor->hasAllComplianceDocuments();
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
