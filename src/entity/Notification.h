#ifndef NOTIFICATION_H
#define NOTIFICATION_H

#include <QString>
#include <QDateTime>

/**
 * @class Notification
 * @brief System notifications shown on vendor dashboards.
 *
 * Types: waitlist slot available, booking confirmed, booking cancelled.
 * Displayed in reverse chronological order on dashboard.
 *
 * @author Osasuyi
 */
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

    int getId() const;
    NotificationType getType() const;
    QString getTypeString() const;
    QString getMessage() const;
    QDateTime getTimestamp() const;
    bool isRead() const;

    void setId(int id);
    void setType(NotificationType type);
    void setMessage(const QString& message);
    void setTimestamp(const QDateTime& time);
    void markAsRead();

private:
    int m_id;
    NotificationType m_type;
    QString m_message;
    QDateTime m_timestamp;
    bool m_isRead;
};

#endif // NOTIFICATION_H
