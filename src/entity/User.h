#ifndef USER_H
#define USER_H

#include <QString>

/**
 * @class User
 * @brief Base class for all user types in HintonMarket.
 *
 * Stores common info like name and username. Subclassed by
 * Vendor, MarketOperator, and SystemAdmin.
 *
 * @author Sheng
 */
class User {
public:
    enum class UserType {
        VENDOR,
        MARKET_OPERATOR,
        SYSTEM_ADMIN
    };

    User();
    User(const QString& name, const QString& username, UserType type);
    virtual ~User();

    // Getters
    int getId() const;
    QString getName() const;
    QString getUsername() const;
    UserType getUserType() const;

    // Setters
    void setId(int id);
    void setName(const QString& name);
    void setUsername(const QString& username);

protected:
    int m_id;            // DB primary key
    QString m_name;
    QString m_username;
    UserType m_userType;
};

#endif // USER_H
