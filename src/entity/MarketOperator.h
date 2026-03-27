#ifndef MARKETOPERATOR_H
#define MARKETOPERATOR_H

#include "User.h"

/**
 * @class MarketOperator
 * @brief User type for market staff who manage vendor operations.
 * @author Victor
 */
class MarketOperator : public User {
public:
    MarketOperator();
    MarketOperator(const QString& name, const QString& username);
    ~MarketOperator();
};

#endif // MARKETOPERATOR_H
