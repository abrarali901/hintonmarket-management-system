// SystemAdmin implementation (Victor)
#include "SystemAdmin.h"

SystemAdmin::SystemAdmin()
    : User("", "", UserType::SYSTEM_ADMIN) {
}

SystemAdmin::SystemAdmin(const QString& name, const QString& username)
    : User(name, username, UserType::SYSTEM_ADMIN) {
}

SystemAdmin::~SystemAdmin() {
}
