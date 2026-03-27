#ifndef AUTHENTICATIONCONTROLLER_H
#define AUTHENTICATIONCONTROLLER_H

#include <QString>

class User;

/**
 * @class AuthenticationController
 * @brief Handles user login/logout by verifying username against DataManager.
 *
 * Not true authentication - just looks up username in the user list
 * and returns the corresponding User object.
 *
 * @author Sheng
 */
class AuthenticationController {
public:
    AuthenticationController();
    ~AuthenticationController();

    User* login(const QString& username);
    void logout();
    User* getCurrentUser() const;

    bool isCurrentUserVendor() const;
    bool isCurrentUserOperator() const;
    bool isCurrentUserAdmin() const;

private:
    User* m_currentUser;
};

#endif // AUTHENTICATIONCONTROLLER_H
