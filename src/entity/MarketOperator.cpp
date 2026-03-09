#include "MarketOperator.h"

MarketOperator::MarketOperator()
    : User("", "", UserType::MARKET_OPERATOR) {
}

MarketOperator::MarketOperator(const QString& name, const QString& username)
    : User(name, username, UserType::MARKET_OPERATOR) {
}

MarketOperator::~MarketOperator() {
}
