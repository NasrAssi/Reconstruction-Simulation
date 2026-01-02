#include "Settlement.h"

Settlement:: Settlement(const string &name, SettlementType type)
    : name(name), type(type){}
    
const string& Settlement::getName() const{
    return name;
}

SettlementType Settlement::getType() const{
    return type;
}

const string Settlement::toString() const{
    return "Settlement: " + getName();
}
//helper method.
int Settlement::getBuildCapacity() const{
    if (type == SettlementType:: VILLAGE) {
        return 1;
    }else if (type == SettlementType:: CITY) {
        return 2;
    }else {
        return 3;
    }
}
