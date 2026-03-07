#ifndef USER_H
#define USER_H

#include <QString>

class User {
public:
    enum class UserType {
        VENDOR,
        MARKET_OPERATOR,
        SYSTEM_ADMIN
    };

    User();
    // '&' uses the original string
    // 'const' no modification will happen
    User(const QString& name, const QString& username, UserType type);
    virtual ~User();

    QString getName() const;
    QString getUsername() const;
    UserType getUserType() const;

    void setName(const QString& name);
    void setUsername(const QString& username);

protected:
    QString m_name;
    QString m_username;
    UserType m_userType;
};

#endif // USER_H