// Notification implementation (Osasuyi)
#include "Notification.h"

Notification::Notification()
    : m_id(0), m_type(NotificationType::BOOKING_CONFIRMED), m_message(""),
      m_timestamp(QDateTime::currentDateTime()), m_isRead(false) {
}

Notification::Notification(NotificationType type, const QString& message)
    : m_id(0), m_type(type), m_message(message),
      m_timestamp(QDateTime::currentDateTime()), m_isRead(false) {
}

Notification::~Notification() {
}

int Notification::getId() const { return m_id; }

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

void Notification::setId(int id) { m_id = id; }
void Notification::setType(NotificationType type) { m_type = type; }

void Notification::setMessage(const QString& message) {
    m_message = message;
}

void Notification::setTimestamp(const QDateTime& time) {
    m_timestamp = time;
}

void Notification::markAsRead() {
    m_isRead = true;
}
