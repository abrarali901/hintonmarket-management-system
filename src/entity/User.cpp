#include "User.h"

// -- Default constructor --
User::User()
    : m_id(0), m_name(""), m_username(""), m_userType(UserType::VENDOR) {
}

// -- Parameterized constructor --
User::User(const QString& name, const QString& username, UserType type)
    : m_id(0), m_name(name), m_username(username), m_userType(type) {
}

User::~User() {
}

int User::getId() const { return m_id; }
QString User::getName() const { return m_name; }
QString User::getUsername() const { return m_username; }
User::UserType User::getUserType() const { return m_userType; }

void User::setId(int id) { m_id = id; }
void User::setName(const QString& name) { m_name = name; }
void User::setUsername(const QString& username) { m_username = username; }
