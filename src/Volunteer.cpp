#include "Volunteer.h"

Volunteer::Volunteer(int id, const string &name): completedOrderId(NO_ORDER), activeOrderId(NO_ORDER), id(id),name(name),isAvailable(true){

}
int Volunteer::getId() const{
    return id;
}

const string &Volunteer::getName() const{
    return name;
}
int Volunteer::getActiveOrderId() const{
    return activeOrderId;
}
void Volunteer::resetCompletedOrderId(){
    completedOrderId = NO_ORDER;
}
int Volunteer::getCompletedOrderId() const{
    return completedOrderId;
}
bool Volunteer::isBusy() const{
    return !isAvailable;
}
void Volunteer::setIsAvailable(bool s){
    isAvailable = s;
}
Volunteer::~Volunteer(){}



CollectorVolunteer::CollectorVolunteer(int id, string name, int coolDown):Volunteer(id,name), coolDown(coolDown), timeLeft(-1) {

}

CollectorVolunteer *CollectorVolunteer::clone() const {
    return new CollectorVolunteer(*this);
}
string CollectorVolunteer::getVolunteerType(const Volunteer &volunteer){
    return "Collector";
}
void CollectorVolunteer::step() {
    bool indicator = decreaseCoolDown();
    if (indicator == true){
        completedOrderId = activeOrderId;
        activeOrderId = NO_ORDER;
        setIsAvailable(true);
        //if collecting is completed, move order from in process vector to pending vector, keep status
    }
}

int CollectorVolunteer::getCoolDown() const{
    return coolDown;
}

int CollectorVolunteer::getTimeLeft() const{
    return timeLeft;
}

void CollectorVolunteer::setTimeLeft(int timeLeft){
    this->timeLeft = timeLeft;
}

bool CollectorVolunteer::decreaseCoolDown(){
    timeLeft--;
    return timeLeft == 0;}
    //Decrease timeLeft by 1,return true if timeLeft=0,false otherwise
bool CollectorVolunteer::hasOrdersLeft() const{
    return true;
}

bool CollectorVolunteer::canTakeOrder(const Order &order) const{
    return !isBusy()&&order.getStatus()==OrderStatus::PENDING;
}
void CollectorVolunteer::acceptOrder(const Order &order){ 
    if(canTakeOrder(order)){
        activeOrderId=order.getId();
        timeLeft=coolDown; 
        setIsAvailable(false);
        //check if we should move order from pending vector to in process vector? change the order status to COLLECTING
        
    }
}
string CollectorVolunteer::toString() const {
    string out = "VolunteerID: " + std::to_string(getId());
    out += "\nisBusy: ";
    out += (isBusy() ? "true" : "false");
    out += "\nOrderId: ";
    out += (isBusy() ? std::to_string(activeOrderId) + " ": "None");
    out += "\nTimeLeft: ";
    out += (activeOrderId != -1 ? std::to_string(getTimeLeft()) + " ": "None");
    out += "\nOrdersLeft: No Limit\n";
    return out;
}
    

LimitedCollectorVolunteer::LimitedCollectorVolunteer(int id, string name, int coolDown ,int maxOrders):
    CollectorVolunteer(id, name, coolDown), maxOrders(maxOrders), ordersLeft(maxOrders){

}
LimitedCollectorVolunteer *LimitedCollectorVolunteer::clone() const {
    return new LimitedCollectorVolunteer(*this);
}
bool LimitedCollectorVolunteer::hasOrdersLeft() const {
    return ordersLeft > 0;
}
bool LimitedCollectorVolunteer::canTakeOrder(const Order &order) const {
    return !isBusy() && hasOrdersLeft()&&order.getStatus()==OrderStatus::PENDING;
}
void LimitedCollectorVolunteer::acceptOrder(const Order &order) {

    if(canTakeOrder(order)){
        activeOrderId=order.getId();
        setTimeLeft(getCoolDown());
        ordersLeft--;
        setIsAvailable(false);
    }
}


int LimitedCollectorVolunteer::getMaxOrders() const{
    return maxOrders;
}
int LimitedCollectorVolunteer::getNumOrdersLeft() const{
    return ordersLeft;
}
string LimitedCollectorVolunteer::toString() const {
    string out = "VolunteerID: " + std::to_string(getId());
    out += "\nisBusy: ";
    out += (isBusy() ? "true" : "false");
    out += "\nOrderId: ";
    out += (isBusy() ? std::to_string(activeOrderId) + " ": "None");
    out += "\nTimeLeft: ";
    out += (activeOrderId != -1 ? std::to_string(getTimeLeft()) + " ": "None");
    out += "\nOrdersLeft: " + std::to_string(ordersLeft);
    out += "\n";
    return out;
}
string LimitedCollectorVolunteer::getVolunteerType(const Volunteer &volunteer){
    return "LimitedCollectorVolunteer";
}    

DriverVolunteer::DriverVolunteer(int id, string name, int maxDistance, int distancePerStep): Volunteer(id, name),
    maxDistance(maxDistance), distancePerStep(distancePerStep), distanceLeft(){//check this later

}
DriverVolunteer *DriverVolunteer::clone() const {
    return new DriverVolunteer(*this);
}

int DriverVolunteer::getDistanceLeft() const{
    return distanceLeft;
}
string DriverVolunteer::getVolunteerType(const Volunteer &volunteer){
    return "DriverVolunteer";
}    
int DriverVolunteer::getMaxDistance() const {
    return maxDistance;
}
int DriverVolunteer::getDistancePerStep() const{
    return distancePerStep;
}  
bool DriverVolunteer::decreaseDistanceLeft(){
    setDistanceLeft(distanceLeft - distancePerStep);
    if (distanceLeft < 0){
        setDistanceLeft(0);
    }
    return distanceLeft == 0;
}

bool DriverVolunteer::hasOrdersLeft() const {
    return true;
}
bool DriverVolunteer::canTakeOrder(const Order &order) const { //check
    return !isBusy() && order.getDistance() <= maxDistance&&order.getStatus()==OrderStatus::COLLECTING;
} 
void DriverVolunteer::acceptOrder(const Order &order) {
    if (canTakeOrder(order)){
        this->activeOrderId=order.getId();
        setDistanceLeft(order.getDistance());
        setIsAvailable(false);
        //add to the in process vector? change the order status to DELIVERING
    }
}
void DriverVolunteer::step() {
    if (decreaseDistanceLeft() == true){
        completedOrderId = activeOrderId;
        activeOrderId = NO_ORDER;
        setIsAvailable(true);
        //if completed, move order from in process vector to completed vector, change status to completed
    }
} // Decrease distanceLeft by distancePerStep
string DriverVolunteer::toString() const {
    string out = "VolunteerID: " + std::to_string(getId());
    out += "\nisBusy: ";
    out += (isBusy() ? "true" : "false");
    out += "\nOrderId: ";
    out += (isBusy() ? std::to_string(activeOrderId) + " ": "None");
    out += "\nTimeLeft: ";
    out += (activeOrderId != -1 ? std::to_string(getDistanceLeft()) + " ": "None");
    out += "\nOrdersLeft: No Limit\n";
    return out;
}

void DriverVolunteer::setDistanceLeft(int distance){
    distanceLeft = distance;
}


LimitedDriverVolunteer::LimitedDriverVolunteer(int id, const string &name, int maxDistance, int distancePerStep,int maxOrders):
    DriverVolunteer(id,name,maxDistance,distancePerStep),maxOrders(maxOrders), ordersLeft(maxOrders), distanceLeft(){
}
LimitedDriverVolunteer *LimitedDriverVolunteer::clone() const {
    return new LimitedDriverVolunteer(*this);
}
int LimitedDriverVolunteer::getMaxOrders() const{
    return maxOrders;
}
string LimitedDriverVolunteer::getVolunteerType(const Volunteer &volunteer){
    return "LimitedDriverVolunteer";
} 
int LimitedDriverVolunteer::getNumOrdersLeft() const{
    return ordersLeft;
}
bool LimitedDriverVolunteer::hasOrdersLeft() const {
    return ordersLeft>0;
}
bool LimitedDriverVolunteer::canTakeOrder(const Order &order) const {
    return !isBusy()&&getMaxDistance()>=order.getDistance()&&hasOrdersLeft()&&order.getStatus()==OrderStatus::COLLECTING;
} // Signal if the volunteer is not busy, the order is within the maxDistance and have orders left

void LimitedDriverVolunteer::acceptOrder(const Order &order) {
    if (canTakeOrder(order)){
        activeOrderId = order.getId();
        setDistanceLeft(order.getDistance());
        ordersLeft--;
        setIsAvailable(false);

}} // Reset distanceLeft to maxDistance and decrease ordersLeft

string LimitedDriverVolunteer::toString() const {
    string out = "VolunteerID: " + std::to_string(getId());
    out += "\nisBusy: ";
    out += (isBusy() ? "true" : "false");
    out += "\nOrderId: ";
    out += (isBusy() ? std::to_string(activeOrderId) + "": "None");
    out += "\nTimeLeft: ";
    out += (activeOrderId != -1 ? std::to_string(getDistanceLeft()) + "": "None");
    out += "\nOrdersLeft: " + std::to_string(getNumOrdersLeft());
    out += "\n";
    return out;
}