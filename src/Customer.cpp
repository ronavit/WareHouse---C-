#include "Customer.h"
#include <iostream>
#include <fstream>
#include <sstream>

Customer::Customer(int id, const string &name, int locationDistance, int maxOrders): id(id), name(name), 
                    locationDistance(locationDistance), maxOrders(maxOrders), ordersId(), numOrdersCustomer(0){
}

const string &Customer::getName() const{
    return name;
}
int Customer::getId() const{
    return id;
}

int Customer::getCustomerDistance() const{
    return locationDistance;
}

int Customer::getMaxOrders() const{
    return maxOrders;
} 

int Customer::getNumOrders() const{
    return numOrdersCustomer;
} //Returns num of orders the customer has made so far

bool Customer::canMakeOrder() const{
    return numOrdersCustomer < maxOrders;
} //Returns true if the customer didn't reach max orders

const vector<int> &Customer::getOrdersIds() const{
    return ordersId;
}
int Customer::getNumOrdersLeft(){
    return (maxOrders - numOrdersCustomer);
}

int Customer::addOrder(int orderId){ 
    if(canMakeOrder() && orderId > -1) {
        ordersId.push_back(orderId); //we add the new ID order to the list
        numOrdersCustomer++;
        return orderId;
    }
    else {
    return -1;
    }
}
Customer::~Customer(){}

SoldierCustomer::SoldierCustomer(int id, string name, int locationDistance,int maxOrders):
                                    Customer(id, name, locationDistance, maxOrders){}

SoldierCustomer *SoldierCustomer::clone() const {
    return new SoldierCustomer(*this);
}
    
CivilianCustomer::CivilianCustomer(int id, string name, int locationDistance, int maxOrders): 
                                    Customer(id, name, locationDistance, maxOrders) {
}
CivilianCustomer *CivilianCustomer::clone() const {
    return new CivilianCustomer(*this);
}
