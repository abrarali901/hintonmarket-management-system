#ifndef SYSTEMADMIN_H
#define SYSTEMADMIN_H

#include "User.h"

class SystemAdmin : public User {
public:
    SystemAdmin();
    SystemAdmin(const QString& name, const QString& username);
    ~SystemAdmin();
};

#endif // SYSTEMADMIN_H