#include "WareHouse.h"
#include <iostream>
#include <fstream>
#include <sstream>

WareHouse::WareHouse(const string &configFilePath):isOpen(false), actionsLog(), volunteers(),pendingOrders(), 
                    inProcessOrders(), completedOrders(), customers(), customerCounter(0), volunteerCounter(0), orderCounter(0){
    std::ifstream file(configFilePath);
    std::string   line;

    while(std::getline(file, line))
    {
        std::stringstream   linestream(line);
        std::string         data1;
        std::string         data2;
        std::string         data3;
        int                 val1;
        int                 val2;
        int                 val3;

        // If you have truly tab delimited data use getline() with third parameter.
        // If your data is just white space separated data
        // then the operator >> will do (it reads a space separated word into a string).
        std::getline(linestream, data1, ' ');  // read up-to the first tab (discard tab).
        if(data1 == "customer" || data1 == "volunteer"){
            linestream >> data2 >> data3 >> val1 >> val2 >> val3;
            if(data1 == "customer" && data3 == "soldier"){
                customers.push_back(new SoldierCustomer(customerCounter, data2, val1, val2)); 
                customerCounter++;
            }else if(data1 == "customer" && data3 == "civilian"){
                customers.push_back(new CivilianCustomer(customerCounter, data2, val1, val2)); 
                customerCounter++;
            }else if(data3 == "collector"){
                volunteers.push_back(new CollectorVolunteer(volunteerCounter, data2, val1));
                volunteerCounter++;
            }else if(data3 == "limited_collector"){
                volunteers.push_back(new LimitedCollectorVolunteer(volunteerCounter,data2,val1,val2));
                volunteerCounter++;
            }else if(data3 == "driver"){
                volunteers.push_back(new DriverVolunteer(volunteerCounter, data2, val1, val2));
                volunteerCounter++;
            }else{
                volunteers.push_back(new LimitedDriverVolunteer(volunteerCounter, data2, val1, val2, val3));
                volunteerCounter++;
            }
        }
    }
}
WareHouse::WareHouse(const WareHouse &other)
    : isOpen(other.isOpen), actionsLog(), volunteers(), pendingOrders(),
      inProcessOrders(), completedOrders(), customers(), customerCounter(other.customerCounter),
      volunteerCounter(other.volunteerCounter), orderCounter(other.orderCounter) {

    for (unsigned int i = 0; i < other.actionsLog.size(); i++) {
        actionsLog.push_back(other.actionsLog[i]->clone());
    }

    for (unsigned int i = 0; i < other.volunteers.size(); i++) {
        volunteers.push_back(other.volunteers[i]->clone());
    }

    for (unsigned int i = 0; i < other.pendingOrders.size(); i++) {
        pendingOrders.push_back(new Order(*other.pendingOrders[i]));
    }

    for (unsigned int i = 0; i < other.inProcessOrders.size(); i++) {
        inProcessOrders.push_back(new Order(*other.inProcessOrders[i]));
    }

    for (unsigned int i = 0; i < other.completedOrders.size(); i++) {
        completedOrders.push_back(new Order(*other.completedOrders[i]));
    }

    for (unsigned int i = 0; i < other.customers.size(); i++) {
        customers.push_back(other.customers[i]->clone());
    }
      }

WareHouse& WareHouse::operator=(const WareHouse &other){
    if(this!=&other){
        this->isOpen=isOpen;
        this->customerCounter=customerCounter;
        this->volunteerCounter=volunteerCounter;
        this->orderCounter=orderCounter;

        for(unsigned int i = 0;i<actionsLog.size();i++){
            delete(actionsLog[i]);
        }
        actionsLog.clear();

        for(unsigned int i = 0;i<other.actionsLog.size();i++){
            actionsLog.push_back(other.actionsLog[i]->clone());
        }
        for(unsigned int i = 0;i<volunteers.size();i++){
            delete(volunteers[i]);
        }
        volunteers.clear();

        for(unsigned int i = 0;i<other.volunteers.size();i++){
            volunteers.push_back(other.volunteers[i]->clone());
        }
        for(unsigned int i = 0;i<pendingOrders.size();i++){
            delete(pendingOrders[i]);
        }
        pendingOrders.clear();

        for(unsigned int i = 0;i<other.pendingOrders.size();i++){
            pendingOrders.push_back(new Order(*other.pendingOrders[i]));
    }

        for(unsigned int i = 0;i<inProcessOrders.size();i++){
            delete(inProcessOrders[i]);
        }
        inProcessOrders.clear();

        for(unsigned int i = 0;i<other.inProcessOrders.size();i++){
            inProcessOrders.push_back(new Order(*other.inProcessOrders[i]));
    }

        for(unsigned int i = 0;i<completedOrders.size();i++){
            delete(completedOrders[i]);
        }
        completedOrders.clear();

        for(unsigned int i = 0;i<other.completedOrders.size();i++){
            completedOrders.push_back(new Order(*other.completedOrders[i]));
        }

        for(unsigned int i = 0;i<customers.size();i++){
            delete(customers[i]);
        }
        customers.clear();

        for(unsigned int i = 0;i<other.customers.size();i++){
            customers.push_back(other.customers[i]->clone());
        }    
    }
    return *this;
}

WareHouse::WareHouse(WareHouse &&other):isOpen(other.isOpen), actionsLog(), volunteers(), pendingOrders(), inProcessOrders(), 
completedOrders(), customers(), customerCounter(other.customerCounter), 
    volunteerCounter(other.volunteerCounter), orderCounter(other.orderCounter){ //move copy constructor

    for(unsigned int i = 0;i<other.actionsLog.size();i++){
        actionsLog.push_back(other.actionsLog[i]);
        other.actionsLog[i] = nullptr;
    }
    for(unsigned int i = 0;i<other.volunteers.size();i++){
        volunteers.push_back(other.volunteers[i]);
        other.volunteers[i] = nullptr;
    }
    for(unsigned int i = 0;i<other.pendingOrders.size();i++){
        pendingOrders.push_back(other.pendingOrders[i]);
        other.pendingOrders[i] = nullptr;
    }
    for(unsigned int i = 0;i<other.inProcessOrders.size();i++){
        inProcessOrders.push_back(other.inProcessOrders[i]);
        other.inProcessOrders[i] = nullptr;
    }
    for(unsigned int i = 0;i<other.completedOrders.size();i++){
        completedOrders.push_back(other.completedOrders[i]);
        other.completedOrders[i] = nullptr;
    }
    for(unsigned int i = 0;i<other.customers.size();i++){
        customers.push_back(other.customers[i]);
        other.customers[i] = nullptr;
    }

        
    }
WareHouse& WareHouse::operator=(WareHouse &&other){// move assignment operator
    if(this!=&other){
        this->isOpen=isOpen;
        this->customerCounter=customerCounter;
        this->volunteerCounter=volunteerCounter;
        this->orderCounter=orderCounter;

        for(unsigned int i = 0;i<actionsLog.size();i++){
            delete(actionsLog[i]);
        }
        actionsLog.clear();

        for(unsigned int i = 0;i<other.actionsLog.size();i++){
            actionsLog.push_back(other.actionsLog[i]);
            other.actionsLog[i] = nullptr;
    }
        for(unsigned int i = 0;i<volunteers.size();i++){
            delete(volunteers[i]);
        }
        volunteers.clear();

        for(unsigned int i = 0;i<other.volunteers.size();i++){
            volunteers.push_back(other.volunteers[i]);
            other.volunteers[i] = nullptr;
    }
        for(unsigned int i = 0;i<pendingOrders.size();i++){
            delete(pendingOrders[i]);
        }
        pendingOrders.clear();

        for(unsigned int i = 0;i<other.pendingOrders.size();i++){
            pendingOrders.push_back(other.pendingOrders[i]);
            other.pendingOrders[i] = nullptr;
    }

        for(unsigned int i = 0;i<inProcessOrders.size();i++){
            delete(inProcessOrders[i]);
        }
        inProcessOrders.clear();

        for(unsigned int i = 0;i<other.inProcessOrders.size();i++){
            inProcessOrders.push_back(other.inProcessOrders[i]);
            other.inProcessOrders[i] = nullptr;
    }

        for(unsigned int i = 0;i<completedOrders.size();i++){
            delete(completedOrders[i]);
        }
        completedOrders.clear();

        for(unsigned int i = 0;i<other.completedOrders.size();i++){
            completedOrders.push_back(other.completedOrders[i]);
            other.completedOrders[i] = nullptr;
    }

        for(unsigned int i = 0;i<customers.size();i++){
            delete(customers[i]);
        }
        customers.clear();

        for(unsigned int i = 0;i<other.customers.size();i++){
            customers.push_back(other.customers[i]);
            other.customers[i] = nullptr;
    }

    
    }
    return *this;
}

const vector<BaseAction*> &WareHouse::getActions() const{
    return actionsLog;
} 



void WareHouse::pushCustomer(Customer *c){
    customers.push_back(c);
    WareHouse::customerCounter++;
}

void WareHouse::pushToPending(Order o){
    pendingOrders.push_back(&o);

}
void WareHouse:: pushToInProcess(Order o){
    inProcessOrders.push_back(&o);
}
void  WareHouse:: pushToCompleted(Order o){
    completedOrders.push_back(&o);
}

const int WareHouse::getOrderCounter(){
    return orderCounter;
}

const int WareHouse::getCustomerCounter(){
    return customerCounter;
}
 
void WareHouse::start(){
    open();
    std::cout << "Warehouse is open!" << std::endl;
    //add input (cin)
    while(isOpen){
        BaseAction* act;
        string line;
        getline(cin, line);
        istringstream stream1(line);
        string action;
        int val;
        stream1 >> action;
        if(action != "customer"){
            stream1 >> val;
            if(action == "order"){
                act = new AddOrder(val);
            } else if(action == "step"){
                act = new SimulateStep(val);
            } else if(action == "log"){
                act = new PrintActionsLog();
            } else if(action == "close"){
                act = new Close();
            } else if(action == "backup"){
                act = new BackupWareHouse();
            } else if(action == "restore"){
                act = new RestoreWareHouse();
            } else if(action == "customerStatus"){
                act = new PrintCustomerStatus(val);
            } else if(action == "volunteerStatus"){
                act = new PrintVolunteerStatus(val);
            } else if(action == "orderStatus"){
                act = new PrintOrderStatus(val);
            }
        } else{
            
            string name, type;
            int val1, val2;
            stream1 >> name >> type >> val1 >> val2;
            act = new AddCustomer(name, type, val1, val2);
        }
        act->act(*this);
        actionsLog.push_back(act);

    }
}

void WareHouse::addOrder(Order* order){
    for(Customer* c : customers ){
        if(order->getCustomerId() == c->getId()){
            if(c->canMakeOrder()){
                pendingOrders.push_back(order);
                WareHouse::orderCounter++;
                }}}}

void WareHouse::addAction(BaseAction* action){
    actionsLog.push_back(action);
    
}

Customer &WareHouse::getCustomer(int customerId) const{
    for(unsigned int i = 0; i < customers.size(); i++){
        if(customers[i]->getId() == customerId)
            return *customers[i];

    }
    static SoldierCustomer fic = SoldierCustomer(-1, "Default", -1, -1);//in case there's no such customer id
    return fic;
}

Volunteer &WareHouse::getVolunteer(int volunteerId) const{
    for(unsigned int i = 0; i < volunteers.size(); i++){
        if(volunteers[i]->getId() == volunteerId)
            return *volunteers[i];
    }
    static CollectorVolunteer fic = CollectorVolunteer(-1, "Default", -1);//in case there's no such customer id
    return fic;
    
}

Order &WareHouse::getOrder(int orderId) const{
    for(unsigned int i = 0; i < pendingOrders.size(); i++){
        if(pendingOrders[i]->getId() == orderId)
            return *pendingOrders[i];
    }    
    for(unsigned int i = 0; i < inProcessOrders.size(); i++){
        if(inProcessOrders[i]->getId() == orderId)
            return *inProcessOrders[i];
    }    
    for(unsigned int i = 0; i < completedOrders.size(); i++){
        if(completedOrders[i]->getId() == orderId)
            return *completedOrders[i];
    }  
    static Order fic = Order(-1, -1, -1); //in case there's no such order id
    return fic;  
}

void WareHouse::close(){
    
    for(unsigned int i = 0; i < pendingOrders.size(); i++){
        std::cout << pendingOrders[i]->toString()<<endl;
    }    
    for(unsigned int i = 0; i < inProcessOrders.size(); i++){
        std::cout << inProcessOrders[i]->toString()<<endl;
    }
    for(unsigned int i = 0; i < completedOrders.size(); i++){
        std::cout << completedOrders[i]->toString()<<endl;
    } 
    isOpen = false;
}




void WareHouse::open(){
    isOpen = true;
    }

WareHouse::~WareHouse(){
    for(unsigned int i = 0; i < actionsLog.size(); i++){
        if (actionsLog.at(i) != nullptr){
            delete(actionsLog.at(i));
        }

    }
    actionsLog.clear();
    for(unsigned int i = 0; i < volunteers.size(); i++){
        if (volunteers.at(i) != nullptr){
            std::cout<<std::to_string(volunteers.at(i)->getId());
            delete(volunteers.at(i));
        }
    }
    volunteers.clear();
    for(unsigned int i = 0;i<pendingOrders.size();i++){
        if (pendingOrders.at(i) != nullptr){
            delete(pendingOrders.at(i));
        }
    }
    pendingOrders.clear();
    for(unsigned int i = 0;i<inProcessOrders.size();i++){
        if (inProcessOrders.at(i) != nullptr){
            delete(inProcessOrders.at(i));
        }
    }
    inProcessOrders.clear();
    for(unsigned int i = 0;i<completedOrders.size();i++){
        if (completedOrders.at(i) != nullptr){
            delete(completedOrders.at(i));
        }
    }
    completedOrders.clear();
    for(unsigned int i = 0;i<customers.size();i++){
        if (customers.at(i) != nullptr){
            delete(customers).at(i);
       }
    }
    customers.clear();

}


void WareHouse::step1() {
    for (auto it = pendingOrders.begin(); it != pendingOrders.end();) {

        bool orderProcessed = false;
        for (unsigned int j = 0; j < volunteers.size(); j++) {
            if (volunteers[j]->canTakeOrder(*(*it))) {
                volunteers[j]->acceptOrder(**it);
                if (convertEnumToString((*it)->getStatus()) == "PENDING") {
                    (*it)->setStatus(OrderStatus::COLLECTING);
                    (*it)->setCollectorId(volunteers[j]->getId());
                    inProcessOrders.push_back(*it);
                    it = pendingOrders.erase(it); 
                    orderProcessed = true;
                    break; 
                } else if (convertEnumToString((*it)->getStatus()) == "COLLECTING") {
                    (*it)->setStatus(OrderStatus::DELIVERING);
                    (*it)->setDriverId(volunteers[j]->getId());
                    inProcessOrders.push_back(*it);
                    it = pendingOrders.erase(it); 
                    orderProcessed = true;
                    break;
                }
            }
        }
        if (!orderProcessed) {
            ++it; 
        }
    }
}



void WareHouse::step2(){
    for(unsigned int j = 0; j < volunteers.size(); j++){
        if(volunteers[j]->isBusy()){
            volunteers[j]->step();
    }}
}


void WareHouse::step3() {
    for (unsigned int j = 0; j < volunteers.size(); j++) {
        if (volunteers[j]->getActiveOrderId() == -1 && volunteers[j]->getCompletedOrderId() != -1 && volunteers[j]->isBusy() == false) {
            
            if (convertEnumToString(getOrder(volunteers[j]->getCompletedOrderId()).getStatus()) == "COLLECTING") {
                for (auto it = inProcessOrders.begin(); it != inProcessOrders.end(); ++it) {
                    if ((*it)->getId() == volunteers[j]->getCompletedOrderId()) {
                        pendingOrders.push_back(*it);
                        it = inProcessOrders.erase(it);
                        volunteers[j]->resetCompletedOrderId();
                        break; 
                    }
                }
            } else if (convertEnumToString(getOrder(volunteers[j]->getCompletedOrderId()).getStatus()) == "DELIVERING") {
                getOrder(volunteers[j]->getCompletedOrderId()).setStatus(OrderStatus::COMPLETED);
                for (auto it = inProcessOrders.begin(); it != inProcessOrders.end(); ++it) {
                    if ((*it)->getId() == volunteers[j]->getCompletedOrderId()) {
                        completedOrders.push_back(*it);
                        it = inProcessOrders.erase(it); 
                        volunteers[j]->resetCompletedOrderId();
                        break; 
                    }
                }
            }
        }
    }
}


void WareHouse::step4() {
    for (auto it = volunteers.begin(); it != volunteers.end();) {
        if (!(*it)->hasOrdersLeft()) {
            delete *it; 
            it = volunteers.erase(it); 
        } else {
            ++it;
        }
    }
}
const string WareHouse::convertEnumToString(OrderStatus s){
    switch (s)
    {
    case OrderStatus::PENDING:
        return "PENDING";

    case OrderStatus::COLLECTING:
        return "COLLECTING";

    case OrderStatus::DELIVERING:
        return "DELIVERING";   

    case OrderStatus::COMPLETED:
        return "COMPLETED";       
    
    }
return "UNKNOWN";

}
