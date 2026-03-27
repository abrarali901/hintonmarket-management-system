#ifndef SYSTEMADMIN_H
#define SYSTEMADMIN_H

#include "User.h"

/**
 * @class SystemAdmin
 * @brief User type for system administrators (not implemented in D2).
 * @author Victor
 */
class SystemAdmin : public User {
public:
    SystemAdmin();
    SystemAdmin(const QString& name, const QString& username);
    ~SystemAdmin();
};

#endif // SYSTEMADMIN_H
