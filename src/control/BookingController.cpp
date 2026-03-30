/**
 * @file BookingController.cpp
 * @brief Booking/cancellation logic with SQLite persistence.
 *
 * D2 change: all book/cancel ops now write to DB via DatabaseManager.
 * Waitlist notifications are also persisted.
 *
 * @author Ali, Victor
 */
#include "BookingController.h"
#include "DataManager.h"
#include "DatabaseManager.h"
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

    // Check availability for vendor's category
    if (vendor->getCategory() == Vendor::VendorCategory::FOOD) {
        if (!date->hasFoodStallAvailable()) return false;
    } else {
        if (!date->hasArtisanStallAvailable()) return false;
    }

    // Check vendor doesn't already have a booking for THIS specific date
    for (StallBooking* existing : vendor->getBookings()) {
        if (existing->getMarketDate() == date) return false;
    }

    // D2 Fix: Enforce waitlist priority - if there are vendors waitlisted
    // for this date/category, only the first-in-queue vendor can book.
    // This prevents other vendors from bypassing the waitlist.
    QVector<WaitlistEntry*>& waitEntries = date->getWaitlistEntries();
    WaitlistEntry* firstInQueue = nullptr;
    for (WaitlistEntry* entry : waitEntries) {
        if (entry->getVendor()->getCategory() == vendor->getCategory()) {
            if (!firstInQueue || entry->getPosition() < firstInQueue->getPosition()) {
                firstInQueue = entry;
            }
        }
    }
    // If a waitlist exists for this category, only the first-in-queue can book
    if (firstInQueue && firstInQueue->getVendor() != vendor) {
        return false;
    }

    // Persist to SQLite first
    int dbId = DatabaseManager::instance().insertBooking(vendor->getId(), date->getId());
    if (dbId < 0) return false;

    // Create in-memory booking object
    StallBooking* booking = new StallBooking(vendor, date);
    booking->setId(dbId);

    // Add to DataManager, MarketDate, and Vendor
    DataManager::instance().addBooking(booking);
    date->addBooking(booking);
    vendor->addBooking(booking);

    // If this vendor was on the waitlist, remove them from it automatically
    if (firstInQueue && firstInQueue->getVendor() == vendor) {
        // Remove from DB
        DatabaseManager::instance().deleteWaitlistEntry(firstInQueue->getId());
        // Remove from in-memory structures
        DataManager::instance().removeWaitlistEntry(firstInQueue);
        date->removeWaitlistEntry(firstInQueue);
        vendor->removeWaitlistEntry(firstInQueue);

        // Update positions for remaining waitlist entries in this category
        Vendor::VendorCategory cat = vendor->getCategory();
        int pos = 1;
        for (WaitlistEntry* e : date->getWaitlistEntries()) {
            if (e->getVendor()->getCategory() == cat) {
                e->setPosition(pos);
                DatabaseManager::instance().updateWaitlistPosition(e->getId(), pos);
                pos++;
            }
        }

        delete firstInQueue;
    }

    // Notify vendor
    notifyBookingConfirmed(vendor, date);

    return true;
}

bool BookingController::cancelBooking(StallBooking* booking) {
    if (!booking) return false;

    Vendor* vendor = booking->getVendor();
    MarketDate* date = booking->getMarketDate();
    Vendor::VendorCategory category = vendor->getCategory();

    // Delete from SQLite first
    DatabaseManager::instance().deleteBooking(booking->getId());

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

        // Persist notification to SQLite
        int notifId = DatabaseManager::instance().insertNotification(
            waitVendor->getId(),
            static_cast<int>(Notification::NotificationType::WAITLIST_AVAILABLE),
            message);
        notification->setId(notifId);

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

    // Persist notification to SQLite
    int notifId = DatabaseManager::instance().insertNotification(
        vendor->getId(),
        static_cast<int>(Notification::NotificationType::BOOKING_CONFIRMED),
        message);
    notification->setId(notifId);

    vendor->addNotification(notification);
}

void BookingController::notifyBookingCancelled(Vendor* vendor, MarketDate* date) {
    QString message = "Booking cancelled for " + date->getDateString();
    Notification* notification = new Notification(
        Notification::NotificationType::BOOKING_CANCELLED, message);

    // Persist notification to SQLite
    int notifId = DatabaseManager::instance().insertNotification(
        vendor->getId(),
        static_cast<int>(Notification::NotificationType::BOOKING_CANCELLED),
        message);
    notification->setId(notifId);

    vendor->addNotification(notification);
}
