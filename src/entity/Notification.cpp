#include "Notification.h"

Notification::Notification()
    : m_type(NotificationType::BOOKING_CONFIRMED), m_message(""),
      m_timestamp(QDateTime::currentDateTime()), m_isRead(false) {
}

Notification::Notification(NotificationType type, const QString& message)
    : m_type(type), m_message(message),
      m_timestamp(QDateTime::currentDateTime()), m_isRead(false) {
}

Notification::~Notification() {
}

Notification::NotificationType Notification::getType() const {
    return m_type;
}

QString Notification::getTypeString() const {
    switch (m_type) {
        case NotificationType::WAITLIST_AVAILABLE:
            return "Waitlist Available";
        case NotificationType::BOOKING_CONFIRMED:
            return "Booking Confirmed";
        case NotificationType::BOOKING_CANCELLED:
            return "Booking Cancelled";
        default:
            return "Unknown";
    }
}

QString Notification::getMessage() const {
    return m_message;
}

QDateTime Notification::getTimestamp() const {
    return m_timestamp;
}

bool Notification::isRead() const {
    return m_isRead;
}

void Notification::setType(NotificationType type) {
    m_type = type;
}

void Notification::setMessage(const QString& message) {
    m_message = message;
}

void Notification::markAsRead() {
    m_isRead = true;
}