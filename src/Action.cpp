#include "Action.h"

extern WareHouse *backup;


BaseAction::BaseAction() : errorMsg(""), status() {}
ActionStatus BaseAction::getStatus() const
{
    return status;
}

void BaseAction::complete()
{
    status = ActionStatus::COMPLETED;
}
void BaseAction::error(string errorMsg)
{
    status = ActionStatus::ERROR;
    std::cout << errorMsg << endl;
}
string BaseAction::getErrorMsg() const
{
    return errorMsg;
}
void BaseAction::setErrorMsg(string errorMsg)
{
    this->errorMsg = errorMsg;
}
BaseAction::~BaseAction() {}

SimulateStep::SimulateStep(int numOfSteps) : numOfSteps(numOfSteps)
{
    string str = "None";
    setErrorMsg(str);
}
// check

void SimulateStep::act(WareHouse &wareHouse)
{
    for (int i = 0; i < numOfSteps; i++)
    {
        wareHouse.step1();
        wareHouse.step2();
        wareHouse.step3();
        wareHouse.step4();
    }

    complete();
}

std::string SimulateStep::toString() const
{
    string str = "simulateStep " + std::to_string(numOfSteps);
    str += " COMPLETED";
    return str;
}

SimulateStep *SimulateStep::clone() const
{
    return new SimulateStep(*this);
}

AddOrder::AddOrder(int id) : customerId(id)
{
    setErrorMsg("Cannot place this order");
}

void AddOrder::act(WareHouse &wareHouse)
{
    // check if customer exists and if he's available
    if (wareHouse.getCustomer(customerId).getId() == -1 || wareHouse.getCustomer(customerId).canMakeOrder() == false)
    {
        error(getErrorMsg());
    }

    else
    {
        Order *o = new Order(wareHouse.getOrderCounter(), customerId, wareHouse.getCustomer(customerId).getCustomerDistance());
        // std::cout<<std::to_string(o->getId());
        wareHouse.addOrder(o);
        wareHouse.getCustomer(customerId).addOrder(o->getId());
        complete();
    }
}
string AddOrder::toString() const
{
    string str = "order ";
    str += std::to_string(customerId);
    if (getStatus() == ActionStatus::COMPLETED)
    {
        return str += " COMPLETED";
    }
    else
        return str += " ERROR";
}

AddOrder *AddOrder::clone() const
{
    return new AddOrder(*this);
}

AddCustomer::AddCustomer(string customerName, string customerType, int distance, int maxOrders) : customerName(customerName),
                                                                                                  customerType(customerType == "soldier" ? CustomerType::Soldier : CustomerType::Civilian), distance(distance), maxOrders(maxOrders)
{
    setErrorMsg("NONE");
}

void AddCustomer::act(WareHouse &wareHouse)
{
    Customer *c = nullptr;
    if (customerType == CustomerType::Soldier)
    {
        c = new SoldierCustomer(wareHouse.getCustomerCounter(), customerName, distance, maxOrders);
    }
    else
    {
        c = new CivilianCustomer(wareHouse.getCustomerCounter(), customerName, distance, maxOrders);
    }
    wareHouse.pushCustomer(c);
    complete();
}
AddCustomer *AddCustomer::clone() const
{
    return new AddCustomer(*this);
}
string AddCustomer::toString() const
{
    string str = "customer " + customerName + " ";
    if (customerType == CustomerType::Soldier)
    {
        str += "soldier";
    }
    else
    {
        str += "civilian";
    }
    str += " ";
    str += std::to_string(distance) + " ";
    str += std::to_string(maxOrders);
    return str;
}

PrintOrderStatus::PrintOrderStatus(int id) : orderId(id)
{
    setErrorMsg("order doesn't exist");
}

void PrintOrderStatus::act(WareHouse &wareHouse)
{
    if (wareHouse.getOrder(orderId).getId() != -1)
    {
        Order o = wareHouse.getOrder(orderId);
        string str = "OrderId: " + std::to_string(o.getId());

        str += "\nStatus: ";
        if (o.getStatus() == OrderStatus::PENDING)
        {
            str = str + "PENDING ";
            str += "\nCustomerId: " + std::to_string(o.getCustomerId());
            str += "\nCollectorId: None";
            str += "\nDriverId: None";
        }
        else if (o.getStatus() == OrderStatus::COLLECTING)
        {
            str = str + "COLLECTING ";
            str += "\nCustomerId: " + std::to_string(o.getCustomerId());
            str += "\nCollectorId: " + std::to_string(o.getCollectorId());
            str += "\nDriverId: None";
        }
        else if (o.getStatus() == OrderStatus::DELIVERING)
        {
            str = str + "DELIVERING ";
            str += "\nCustomerId: " + std::to_string(o.getCustomerId());
            str += "\nCollectorId: " + std::to_string(o.getCollectorId());
            str += "\nDriverId: " + std::to_string(o.getDriverId());
        }
        else if (o.getStatus() == OrderStatus::COMPLETED)
        {
            str = str + "COMPLETED ";
            str += "\nCustomerId: " + std::to_string(o.getCustomerId());
            str += "\nCollectorId: " + std::to_string(o.getCollectorId());
            str += "\nDriverId: " + std::to_string(o.getDriverId());
        }
        std::cout << str << endl;
        complete();
    }

    else
    {
        error(getErrorMsg());
    }
}
PrintOrderStatus *PrintOrderStatus::clone() const
{
    return new PrintOrderStatus(*this);
}
string PrintOrderStatus::toString() const
{
    string str = "orderStatus " + std::to_string(orderId);
    if (BaseAction::getStatus() == ActionStatus::COMPLETED)
    {
        return str += " COMPLETED";
    }
    else
        return str += " ERROR";
}

PrintCustomerStatus::PrintCustomerStatus(int customerId) : customerId(customerId)
{
    setErrorMsg("Customer doesn't exist");
}
void PrintCustomerStatus::act(WareHouse &wareHouse)
{
    if (wareHouse.getCustomer(customerId).getId() != -1)
    {
        string str = "Customer ID: " + std::to_string(customerId);
        if (wareHouse.getCustomer(customerId).getOrdersIds().size() == 0)
        {
            str += "\nNo Orders yet";
            str += "\nNumOrdersLeft: " + std::to_string(wareHouse.getCustomer(customerId).getNumOrdersLeft());
            std::cout << str << endl;
            complete();
        }
        else
        {
            for (unsigned int i = 0; i < wareHouse.getCustomer(customerId).getOrdersIds().size(); i++)
            {
                str += "\nOrderID: " + std::to_string(wareHouse.getCustomer(customerId).getOrdersIds()[i]);
                Order o = wareHouse.getOrder(wareHouse.getCustomer(customerId).getOrdersIds()[i]);

                if (o.getStatus() == OrderStatus::PENDING)
                {
                    str += "\nOrderStatus: Pending\n";
                }
                else if (o.getStatus() == OrderStatus::COLLECTING)
                {
                    str += "\nOrderStatus: Collecting\n";
                }
                else if (o.getStatus() == OrderStatus::DELIVERING)
                {
                    str += "\nOrderStatus: Delivering\n";
                } 
                else if (o.getStatus() == OrderStatus::COMPLETED)
                {
                    str += "\nOrderStatus: Completed\n";
                }
                std::cout << str << endl;
                
            }
            std::cout<<"\nNumOrdersLeft: "<<std::to_string(wareHouse.getCustomer(customerId).getNumOrdersLeft());
            complete();
        }
    }
    else
    {
        error(getErrorMsg());
    }
}
PrintCustomerStatus *PrintCustomerStatus::clone() const
{
    return new PrintCustomerStatus(*this);
}
string PrintCustomerStatus::toString() const
{
    string str = "customerStatus " + std::to_string(customerId);
    if (getStatus() == ActionStatus::COMPLETED)
    {
        return str + " COMPLETED";
    }
    else
        return str + " ERROR";
}

PrintVolunteerStatus::PrintVolunteerStatus(int id) : volunteerId(id)
{
    setErrorMsg("Volunteer doesn't exist");
}

void PrintVolunteerStatus::act(WareHouse &wareHouse)
{
    if (wareHouse.getVolunteer(volunteerId).getId() != -1)
    {
        cout << wareHouse.getVolunteer(volunteerId).toString() << endl;
        complete();
    }
    else
    {
        error(getErrorMsg());
    }
}
PrintVolunteerStatus *PrintVolunteerStatus::clone() const
{
    return new PrintVolunteerStatus(*this);
}
string PrintVolunteerStatus::toString() const
{
    string str = "volunteerStatus " + std::to_string(volunteerId);
    if (getStatus() == ActionStatus::COMPLETED)
    {
        return str += " COMPLETED";
    }
    else
        return str += " ERROR";
}

PrintActionsLog::PrintActionsLog()
{
    setErrorMsg("NONE");
}

void PrintActionsLog::act(WareHouse &wareHouse)
{
    vector<BaseAction *> action = wareHouse.getActions();
    for (unsigned int i = 0; i < action.size(); i++)
    {
        std::cout << action[i]->toString() << endl;
    }
    complete();
}
PrintActionsLog *PrintActionsLog::clone() const
{
    return new PrintActionsLog(*this);
}
string PrintActionsLog::toString() const
{
    return "log COMPLETED";
}

Close::Close() {}
void Close::act(WareHouse &wareHouse)
{
    wareHouse.close();
}
string Close::toString() const
{
    string str = "Closed";
    return str;
}
Close *Close::clone() const
{
    return new Close(*this);
}

BackupWareHouse::BackupWareHouse() {}
void BackupWareHouse::act(WareHouse &wareHouse)
{

    if (backup != nullptr)
    {
        delete backup;
        backup = nullptr;
    }
    backup = new WareHouse(wareHouse);
    complete();
}
BackupWareHouse *BackupWareHouse::clone() const
{
    return new BackupWareHouse(*this);
}
string BackupWareHouse::toString() const
{
    string str = "Backup COMPLETED";
    return str;
}

RestoreWareHouse::RestoreWareHouse()
{
    setErrorMsg("No backup available");
}
void RestoreWareHouse::act(WareHouse &wareHouse)
{
    if (backup != nullptr)
    {
        wareHouse = *backup;
        complete();
    }
    else
    {
        error(getErrorMsg());
    }
}
RestoreWareHouse *RestoreWareHouse::clone() const
{
    return new RestoreWareHouse(*this);
}

string RestoreWareHouse::toString() const
{
    string str = "Restore ";
    if (getStatus() == ActionStatus::COMPLETED)
    {
        return str += "COMPLETED";
    }
    else
    {
        return str += "ERROR";
    }
}
