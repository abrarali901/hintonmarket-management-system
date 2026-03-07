#include "Vendor.h"
#include "ComplianceDocument.h"
#include "StallBooking.h"
#include "WaitlistEntry.h"
#include "Notification.h"

Vendor::Vendor()
    : User(), m_businessName(""), m_email(""), m_phone(""),
      m_address(""), m_category(VendorCategory::FOOD) {
}

Vendor::Vendor(const QString& name, const QString& username,
               const QString& businessName, const QString& email,
               const QString& phone, const QString& address,
               VendorCategory category)
    : User(name, username, UserType::VENDOR),
      m_businessName(businessName), m_email(email), m_phone(phone),
      m_address(address), m_category(category) {
}

Vendor::~Vendor() {
}

QString Vendor::getBusinessName() const {
    return m_businessName;
}

QString Vendor::getEmail() const {
    return m_email;
}

QString Vendor::getPhone() const {
    return m_phone;
}

QString Vendor::getAddress() const {
    return m_address;
}

Vendor::VendorCategory Vendor::getCategory() const {
    return m_category;
}

QString Vendor::getCategoryString() const {
    return (m_category == VendorCategory::FOOD) ? "Food" : "Artisan";
}

void Vendor::setBusinessName(const QString& businessName) {
    m_businessName = businessName;
}

void Vendor::setEmail(const QString& email) {
    m_email = email;
}

void Vendor::setPhone(const QString& phone) {
    m_phone = phone;
}

void Vendor::setAddress(const QString& address) {
    m_address = address;
}

QVector<ComplianceDocument*>& Vendor::getComplianceDocuments() {
    return m_complianceDocuments;
}

void Vendor::addComplianceDocument(ComplianceDocument* doc) {
    m_complianceDocuments.append(doc);
}

QVector<StallBooking*>& Vendor::getBookings() {
    return m_bookings;
}

void Vendor::addBooking(StallBooking* booking) {
    m_bookings.append(booking);
}

void Vendor::removeBooking(StallBooking* booking) {
    m_bookings.removeOne(booking);
}

QVector<WaitlistEntry*>& Vendor::getWaitlistEntries() {
    return m_waitlistEntries;
}

void Vendor::addWaitlistEntry(WaitlistEntry* entry) {
    m_waitlistEntries.append(entry);
}

void Vendor::removeWaitlistEntry(WaitlistEntry* entry) {
    m_waitlistEntries.removeOne(entry);
}

QVector<Notification*>& Vendor::getNotifications() {
    return m_notifications;
}

void Vendor::addNotification(Notification* notification) {
    m_notifications.append(notification);
}