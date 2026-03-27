#ifndef VENDOR_H
#define VENDOR_H

#include "User.h"
#include <QString>
#include <QVector>

class ComplianceDocument;
class StallBooking;
class WaitlistEntry;
class Notification;

/**
 * @class Vendor
 * @brief Represents a food or artisan vendor in the market.
 *
 * Holds business info, compliance documents, bookings, waitlist positions
 * and notifications. Inherits from User.
 *
 * @author Ali
 */
class Vendor : public User {
public:
    enum class VendorCategory {
        FOOD,
        ARTISAN
    };

    Vendor();
    Vendor(const QString& name, const QString& username,
           const QString& businessName, const QString& email,
           const QString& phone, const QString& address,
           VendorCategory category);
    ~Vendor();

    // Business info getters
    QString getBusinessName() const;
    QString getEmail() const;
    QString getPhone() const;
    QString getAddress() const;
    VendorCategory getCategory() const;
    QString getCategoryString() const;

    // Business info setters
    void setBusinessName(const QString& businessName);
    void setEmail(const QString& email);
    void setPhone(const QString& phone);
    void setAddress(const QString& address);

    // Compliance document management
    QVector<ComplianceDocument*>& getComplianceDocuments();
    void addComplianceDocument(ComplianceDocument* doc);

    // Booking management
    QVector<StallBooking*>& getBookings();
    void addBooking(StallBooking* booking);
    void removeBooking(StallBooking* booking);

    // Waitlist management
    QVector<WaitlistEntry*>& getWaitlistEntries();
    void addWaitlistEntry(WaitlistEntry* entry);
    void removeWaitlistEntry(WaitlistEntry* entry);

    // Notification management
    QVector<Notification*>& getNotifications();
    void addNotification(Notification* notification);

    // Checks if vendor has all required documents for their category
    bool hasAllComplianceDocuments() const;

private:
    QString m_businessName;
    QString m_email;
    QString m_phone;
    QString m_address;
    VendorCategory m_category;

    QVector<ComplianceDocument*> m_complianceDocuments;
    QVector<StallBooking*> m_bookings;
    QVector<WaitlistEntry*> m_waitlistEntries;
    QVector<Notification*> m_notifications;
};

#endif // VENDOR_H
