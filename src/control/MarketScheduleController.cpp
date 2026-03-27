// MarketScheduleController implementation (Sheng)
#include "MarketScheduleController.h"
#include "DataManager.h"
#include "MarketDate.h"
#include "Vendor.h"
#include <QDate>

MarketScheduleController::MarketScheduleController() {
}

MarketScheduleController::~MarketScheduleController() {
}

QVector<MarketDate*> MarketScheduleController::getAllMarketDates() {
    return DataManager::instance().getMarketDates();
}

QVector<MarketDate*> MarketScheduleController::getAvailableMarketDates() {
    QVector<MarketDate*> available;
    QVector<MarketDate*>& allDates = DataManager::instance().getMarketDates();

    for (MarketDate* date : allDates) {
        if (isWithinBookingWindow(date)) {
            available.append(date);
        }
    }
    return available;
}

bool MarketScheduleController::hasAvailableStall(MarketDate* date, Vendor* vendor) {
    if (!date || !vendor) return false;

    if (vendor->getCategory() == Vendor::VendorCategory::FOOD) {
        return date->hasFoodStallAvailable();
    } else {
        return date->hasArtisanStallAvailable();
    }
}

bool MarketScheduleController::isWithinBookingWindow(MarketDate* date) {
    if (!date) return false;

    QDate today = QDate::currentDate();
    QDate marketDate = date->getDate();

    // Only filter out dates that have already passed
    // Note: booking deadline enforcement (Wednesday 11:59 PM) is
    // explicitly excluded from the D1/D2 prototype per spec
    if (marketDate <= today) return false;

    return true;
}
