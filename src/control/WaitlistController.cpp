/**
 * @file WaitlistController.cpp
 * @brief Waitlist FIFO queue management with SQLite persistence.
 *
 * D2 change: join/leave/position-update ops now persist to DB.
 *
 * @author Ali, Osasuyi
 */
#include "WaitlistController.h"
#include "DataManager.h"
#include "DatabaseManager.h"
#include "Vendor.h"
#include "MarketDate.h"
#include "WaitlistEntry.h"
#include "Notification.h"
#include <algorithm>

WaitlistController::WaitlistController() {
}

WaitlistController::~WaitlistController() {
}

bool WaitlistController::joinWaitlist(Vendor* vendor, MarketDate* date) {
    if (!vendor || !date) return false;

    // Check if already on waitlist for this date
    for (WaitlistEntry* e : vendor->getWaitlistEntries()) {
        if (e->getMarketDate() == date) return false;
    }

    QVector<WaitlistEntry*> waitlist = getWaitlistForDate(date, vendor->getCategory());
    int newPosition = waitlist.size() + 1;

    // Persist to SQLite first
    int dbId = DatabaseManager::instance().insertWaitlistEntry(
        vendor->getId(), date->getId(), newPosition);
    if (dbId < 0) return false;

    // Create in-memory object
    WaitlistEntry* entry = new WaitlistEntry(vendor, date);
    entry->setId(dbId);
    entry->setPosition(newPosition);

    DataManager::instance().addWaitlistEntry(entry);
    date->addWaitlistEntry(entry);
    vendor->addWaitlistEntry(entry);

    return true;
}

bool WaitlistController::leaveWaitlist(WaitlistEntry* entry) {
    if (!entry) return false;

    Vendor* vendor = entry->getVendor();
    MarketDate* date = entry->getMarketDate();
    Vendor::VendorCategory category = vendor->getCategory();

    // Delete from SQLite first
    DatabaseManager::instance().deleteWaitlistEntry(entry->getId());

    DataManager::instance().removeWaitlistEntry(entry);
    date->removeWaitlistEntry(entry);
    vendor->removeWaitlistEntry(entry);

    // Update positions for remaining entries
    updatePositions(date, category);

    // Notify next in queue if applicable
    notifyNextInQueue(date, category);

    delete entry;
    return true;
}

QVector<WaitlistEntry*> WaitlistController::getWaitlistForDate(MarketDate* date, Vendor::VendorCategory category) {
    QVector<WaitlistEntry*> result;
    if (!date) return result;

    QVector<WaitlistEntry*>& entries = date->getWaitlistEntries();
    for (WaitlistEntry* entry : entries) {
        if (entry->getVendor()->getCategory() == category) {
            result.append(entry);
        }
    }

    // Sort by position
    std::sort(result.begin(), result.end(), [](WaitlistEntry* a, WaitlistEntry* b) {
        return a->getPosition() < b->getPosition();
    });

    return result;
}

int WaitlistController::getPosition(Vendor* vendor, MarketDate* date) {
    if (!vendor || !date) return 0;

    for (WaitlistEntry* entry : vendor->getWaitlistEntries()) {
        if (entry->getMarketDate() == date) {
            return entry->getPosition();
        }
    }
    return 0;
}

void WaitlistController::notifyNextInQueue(MarketDate* date, Vendor::VendorCategory category) {
    QVector<WaitlistEntry*> waitlist = getWaitlistForDate(date, category);
    if (waitlist.isEmpty()) return;

    WaitlistEntry* first = waitlist.first();
    Vendor* vendor = first->getVendor();
    QString message = "A stall is now available for " + date->getDateString() +
                     "! You are next in line.";
    Notification* notification = new Notification(
        Notification::NotificationType::WAITLIST_AVAILABLE, message);

    // Persist notification to SQLite
    int notifId = DatabaseManager::instance().insertNotification(
        vendor->getId(),
        static_cast<int>(Notification::NotificationType::WAITLIST_AVAILABLE),
        message);
    notification->setId(notifId);

    vendor->addNotification(notification);
}

void WaitlistController::updatePositions(MarketDate* date, Vendor::VendorCategory category) {
    QVector<WaitlistEntry*> waitlist = getWaitlistForDate(date, category);

    // Sort by join time (FIFO)
    std::sort(waitlist.begin(), waitlist.end(), [](WaitlistEntry* a, WaitlistEntry* b) {
        return a->getJoinTime() < b->getJoinTime();
    });

    int position = 1;
    for (WaitlistEntry* entry : waitlist) {
        entry->setPosition(position);
        // Persist updated position to SQLite
        DatabaseManager::instance().updateWaitlistPosition(entry->getId(), position);
        position++;
    }
}
