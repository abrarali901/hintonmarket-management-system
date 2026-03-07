#include "User.h"

User::User()
    : m_name(""), m_username(""), m_userType(UserType::VENDOR) {
}

User::User(const QString& name, const QString& username, UserType type)
    : m_name(name), m_username(username), m_userType(type) {
}

User::~User() {
}

QString User::getName() const {
    return m_name;
}

QString User::getUsername() const {
    return m_username;
}

User::UserType User::getUserType() const {
    return m_userType;
}

void User::setName(const QString& name) {
    m_name = name;
}

void User::setUsername(const QString& username) {
    m_username = username;
}