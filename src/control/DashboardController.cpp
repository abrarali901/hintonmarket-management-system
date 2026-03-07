#include "DashboardController.h"
#include "Vendor.h"
#include "StallBooking.h"
#include "WaitlistEntry.h"
#include "ComplianceDocument.h"
#include "Notification.h"

DashboardController::DashboardController()
    : m_vendor(nullptr) {
}

DashboardController::~DashboardController() {
}

void DashboardController::setVendor(Vendor* vendor) {
    m_vendor = vendor;
}

Vendor* DashboardController::getVendor() const {
    return m_vendor;
}

QVector<StallBooking*> DashboardController::getBookings() {
    if (!m_vendor) return QVector<StallBooking*>();
    return m_vendor->getBookings();
}

QVector<WaitlistEntry*> DashboardController::getWaitlistEntries() {
    if (!m_vendor) return QVector<WaitlistEntry*>();
    return m_vendor->getWaitlistEntries();
}

QVector<ComplianceDocument*> DashboardController::getComplianceDocuments() {
    if (!m_vendor) return QVector<ComplianceDocument*>();
    return m_vendor->getComplianceDocuments();
}

QVector<Notification*> DashboardController::getNotifications() {
    if (!m_vendor) return QVector<Notification*>();
    return m_vendor->getNotifications();
}

QVector<Notification*> DashboardController::getUnreadNotifications() {
    QVector<Notification*> unread;
    if (!m_vendor) return unread;

    QVector<Notification*>& notifications = m_vendor->getNotifications();
    for (Notification* notification : notifications) {
        if (!notification->isRead()) {
            unread.append(notification);
        }
    }
    return unread;
}

void DashboardController::markNotificationRead(Notification* notification) {
    if (notification) {
        notification->markAsRead();
    }
}