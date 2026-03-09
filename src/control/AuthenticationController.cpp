#include "AuthenticationController.h"
#include "DataManager.h"
#include "User.h"

AuthenticationController::AuthenticationController()
    : m_currentUser(nullptr) {
}

AuthenticationController::~AuthenticationController() {
}

User* AuthenticationController::login(const QString& username) {
    m_currentUser = DataManager::instance().findUserByUsername(username);
    return m_currentUser;
}

void AuthenticationController::logout() {
    m_currentUser = nullptr;
}

User* AuthenticationController::getCurrentUser() const {
    return m_currentUser;
}

bool AuthenticationController::isCurrentUserVendor() const {
    if (!m_currentUser) return false;
    return m_currentUser->getUserType() == User::UserType::VENDOR;
}

bool AuthenticationController::isCurrentUserOperator() const {
    if (!m_currentUser) return false;
    return m_currentUser->getUserType() == User::UserType::MARKET_OPERATOR;
}

bool AuthenticationController::isCurrentUserAdmin() const {
    if (!m_currentUser) return false;
    return m_currentUser->getUserType() == User::UserType::SYSTEM_ADMIN;
}
