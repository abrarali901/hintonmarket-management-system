#ifndef NOTIFICATION_H
#define NOTIFICATION_H

#include <QString>
#include <QDateTime>

class Notification {
public:
    // 3 types of Notification Type
    enum class NotificationType {
        WAITLIST_AVAILABLE,
        BOOKING_CONFIRMED,
        BOOKING_CANCELLED
    };

    Notification();
    Notification(NotificationType type, const QString& message);
    ~Notification();

    // Getters
    NotificationType getType() const;
    QString getTypeString() const;
    QString getMessage() const;
    QDateTime getTimestamp() const;
    bool isRead() const;

    // Setters
    void setType(NotificationType type);
    void setMessage(const QString& message);
    void markAsRead();

private:
    // m_type can only be the one of the allocated enums
    NotificationType m_type;
    QString m_message;
    // when the notification was created
    QDateTime m_timestamp;
    // tracks if the vendor has seen it
    bool m_isRead;
};

#endif // NOTIFICATION_H