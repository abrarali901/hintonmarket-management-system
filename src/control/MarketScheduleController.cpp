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

    // Must be in the future
    if (marketDate <= today) return false;

    // Must be within 4 weeks
    QDate fourWeeksAhead = today.addDays(28);
    if (marketDate > fourWeeksAhead) return false;

    return true;
}