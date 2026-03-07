#include "WaitlistController.h"
#include "DataManager.h"
#include "Vendor.h"
#include "MarketDate.h"
#include "WaitlistEntry.h"
#include "Notification.h"

WaitlistController::WaitlistController() {
}

WaitlistController::~WaitlistController() {
}

bool WaitlistController::joinWaitlist(Vendor* vendor, MarketDate* date) {
    if (!vendor || !date) return false;

    // Get current waitlist for this date and category
    QVector<WaitlistEntry*> waitlist = getWaitlistForDate(date, vendor->getCategory());

    // Create entry with position at end of queue
    WaitlistEntry* entry = new WaitlistEntry(vendor, date);
    entry->setPosition(waitlist.size() + 1);

    // Add to DataManager
    DataManager::instance().addWaitlistEntry(entry);

    // Add to MarketDate
    date->addWaitlistEntry(entry);

    // Add to Vendor
    vendor->addWaitlistEntry(entry);

    return true;
}

bool WaitlistController::leaveWaitlist(WaitlistEntry* entry) {
    if (!entry) return false;

    Vendor* vendor = entry->getVendor();
    MarketDate* date = entry->getMarketDate();
    Vendor::VendorCategory category = vendor->getCategory();

    // Remove from DataManager
    DataManager::instance().removeWaitlistEntry(entry);

    // Remove from MarketDate
    date->removeWaitlistEntry(entry);

    // Remove from Vendor
    vendor->removeWaitlistEntry(entry);

    // Update positions for remaining entries
    updatePositions(date, category);

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
    return result;
}

int WaitlistController::getPosition(Vendor* vendor, MarketDate* date) {
    if (!vendor || !date) return 0;

    QVector<WaitlistEntry*>& entries = vendor->getWaitlistEntries();
    for (WaitlistEntry* entry : entries) {
        if (entry->getMarketDate() == date) {
            return entry->getPosition();
        }
    }
    return 0;
}

void WaitlistController::notifyNextInQueue(MarketDate* date, Vendor::VendorCategory category) {
    QVector<WaitlistEntry*> waitlist = getWaitlistForDate(date, category);

    if (waitlist.isEmpty()) return;

    // Find position 1 (first in queue)
    for (WaitlistEntry* entry : waitlist) {
        if (entry->getPosition() == 1) {
            Vendor* vendor = entry->getVendor();
            QString message = "A stall is now available for " + date->getDateString();
            Notification* notification = new Notification(
                Notification::NotificationType::WAITLIST_AVAILABLE, message);
            vendor->addNotification(notification);
            break;
        }
    }
}

void WaitlistController::updatePositions(MarketDate* date, Vendor::VendorCategory category) {
    QVector<WaitlistEntry*> waitlist = getWaitlistForDate(date, category);

    // Sort by join time and reassign positions
    // TODO: Sort waitlist by join time (FIFO)

    int position = 1;
    for (WaitlistEntry* entry : waitlist) {
        entry->setPosition(position);
        position++;
    }
}