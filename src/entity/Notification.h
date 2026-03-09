#ifndef NOTIFICATION_H
#define NOTIFICATION_H

#include <QString>
#include <QDateTime>

class Notification {
public:
    enum class NotificationType {
        WAITLIST_AVAILABLE,
        BOOKING_CONFIRMED,
        BOOKING_CANCELLED
    };

    Notification();
    Notification(NotificationType type, const QString& message);
    ~Notification();

    NotificationType getType() const;
    QString getTypeString() const;
    QString getMessage() const;
    QDateTime getTimestamp() const;
    bool isRead() const;

    void setType(NotificationType type);
    void setMessage(const QString& message);
    void markAsRead();

private:
    NotificationType m_type;
    QString m_message;
    QDateTime m_timestamp;
    bool m_isRead;
};

#endif // NOTIFICATION_H
