#ifndef MARKETOPERATOR_H
#define MARKETOPERATOR_H

#include "User.h"

class MarketOperator : public User {
public:
    MarketOperator();
    MarketOperator(const QString& name, const QString& username);
    ~MarketOperator();
};

#endif // MARKETOPERATOR_H
