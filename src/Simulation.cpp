#include "Simulation.h"
#include <iostream>
#include <fstream>
#include "Action.h"

// Helper functions.
SelectionPolicy* selectionPolicyFromString(const string& selectionPolicy){
    if (selectionPolicy == "nve") {
        return new NaiveSelection();
    }else if (selectionPolicy == "bal") {
        return new BalancedSelection(0, 0, 0);
    }else if (selectionPolicy == "eco") {
        return new EconomySelection();
    }else {
        return new SustainabilitySelection();
    }
}

//helper method.
const vector<BaseAction*>& Simulation:: GetActionsLog() const{
    return actionsLog;
}

//helper method.
const bool Simulation:: isPlanExists(int planId) const {
    return planId < (int)plans.size() && planId >= 0;
}

Simulation:: Simulation(const string& configFilePath):isRunning(false), planCounter(0),actionsLog(),plans(),settlements(),facilitiesOptions(),unknownSettlement(new Settlement("ThereIsNon", SettlementType:: VILLAGE)), unknownPlan(new Plan(-1, *unknownSettlement, new EconomySelection, facilitiesOptions)) {
    std::ifstream inputFile(configFilePath);   // Open the file for reading

    string line;
    while (std::getline(inputFile, line)) {
        if (!line.empty() && line[0] != '#') {
            vector<string> parsed = Auxiliary:: parseArguments(line);
            if(parsed[0] == "settlement" && parsed.size() == 3){
                this-> addSettlement(new Settlement(parsed[1], (SettlementType) std::stoi(parsed[2])));
            }else if(parsed[0] == "facility" && parsed.size() == 7) {
                this-> addFacility(std::move(FacilityType(parsed[1], (FacilityCategory)std::stoi(parsed[2]), std::stoi(parsed[3]), std::stoi(parsed[4]), std::stoi(parsed[5]), std::stoi(parsed[6]))));
            }else if(parsed[0] == "plan" && parsed.size() == 3) {
                this-> addPlan(getSettlement(parsed[1]), selectionPolicyFromString(parsed[2]));
            }
        }   
    }
    // Close the file
    inputFile.close();
}

void Simulation:: start() {
    isRunning = true;
    cout << "The simulation has started" << endl;

    while (isRunning) {
        string line;
        getline(std::cin, line);

        vector<std::string> command = Auxiliary:: parseArguments(line);
        if (command.empty()) {
            continue;
        }
        BaseAction* action = nullptr;

        if (command[0] == "step" && command.size() == 2) {
            int numOfSteps = std::stoi(command[1]);
            action = new SimulateStep(numOfSteps);
        }
        if (command[0] == "plan" && command.size() == 3) {
            action = new AddPlan(command[1], command[2]);
        }
        if (command[0] == "settlement" && command.size() == 3) {
            SettlementType type = (SettlementType)(std::stoi(command[2]));
            action = new AddSettlement(command[1], type);
        }
        if (command[0] == "facility" && command.size() == 7) {
            action = new AddFacility(command[1], (FacilityCategory)(std::stoi(command[2])), std::stoi(command[3]), std::stoi(command[4]), std::stoi(command[5]), std::stoi(command[6]));
        }
        if (command[0] == "planStatus" && command.size() == 2) {
            int planId = std::stoi(command[1]);
            action = new PrintPlanStatus(planId);
        }
        if (command[0] == "changePolicy" && command.size() == 3) {
            int planId = std::stoi(command[1]);
            action = new ChangePlanPolicy(planId, command[2]);
        }
        if (command[0] == "log") {
            action = new PrintActionsLog();
        }
        if (command[0] == "backup") {
            action = new BackupSimulation();
        }
        if (command[0] == "restore") {
            action = new RestoreSimulation();
        }
        if (command[0] == "close") {
            action = new Close();
            isRunning = false;
        }

        if (action != nullptr) {
            action->act(*this);
        }else {
            cout << "Unknown command: " << command[0] << endl;
        }
    }
    cout << "The simulation has ended" << endl;
}

void Simulation:: addPlan(const Settlement& settlement, SelectionPolicy* selectionPolicy) {
    plans.push_back(std::move(Plan(planCounter, settlement, selectionPolicy, facilitiesOptions)));
    planCounter++;
}

void Simulation:: addAction(BaseAction* action) {
    actionsLog.push_back(action);
}

bool Simulation:: addSettlement(Settlement* settlement){
    if (!isSettlementExists(settlement->getName())) {
        settlements.push_back(settlement);
        return true;
    }
    delete settlement;
    return false;
}

bool Simulation:: addFacility(FacilityType facility){
    for(FacilityType facility1: facilitiesOptions) {
        if (facility1.getName() == facility.getName()) {
            return false;
        }   
    }
    facilitiesOptions.push_back(facility);
    return true;
}

bool Simulation:: isSettlementExists(const string& settlementName) const{
    for (Settlement* settlement: settlements) {
        if (settlement->getName() == settlementName) {
            return true;
        }
    }
    return false;
}

Settlement& Simulation:: getSettlement(const string& settlementName){
    for(Settlement* Settlement: settlements) {
        if (Settlement->getName() == settlementName) {
            return *Settlement;
        }
    }
    return *unknownSettlement;
}

Plan& Simulation:: getPlan(const int planID) {
    if (planID < (int)plans.size()) {
        return plans[planID];
    }
    return *unknownPlan;
}

void Simulation:: step(){
    for(Plan& plan: plans) {
        plan.step();
    }
}

void Simulation:: close(){
    isRunning = false;
    for(const Plan& item: plans) {
        item.printplan();
    }
}

void Simulation:: open() {
    isRunning = true;
    start();
}

//rule of 5.
Simulation:: Simulation(const Simulation& other): isRunning(other.isRunning), planCounter(other.planCounter),actionsLog(),plans(),settlements(), facilitiesOptions(), unknownSettlement(new Settlement("ThereIsNon", SettlementType:: VILLAGE)), unknownPlan(new Plan(-1, *unknownSettlement, new EconomySelection, facilitiesOptions)){
    for (const FacilityType& item: other.facilitiesOptions) {
        this->facilitiesOptions.push_back(item);
    }   
    for (BaseAction* item : other.actionsLog) {
        actionsLog.push_back(item->clone());
    }
    for (Settlement* item: other.settlements) {
        settlements.push_back(new Settlement(*item));
    }
    for (const Plan& item: other.plans) {
        Settlement& newSettlement = getSettlement(item.getSettlement().getName());
        plans.push_back(Plan(item, newSettlement, facilitiesOptions));
    }

}

Simulation& Simulation:: operator= (const Simulation& other) {
    if (this != &other) {
        plans.clear();
        facilitiesOptions.clear();
        
        this->Clean();
        
        isRunning = other.isRunning;
        planCounter = other.planCounter;
        
        unknownSettlement = new Settlement("ThereIsNon", SettlementType::VILLAGE);
        unknownPlan = new Plan(-1, *unknownSettlement, new EconomySelection(), facilitiesOptions);
        
        for (const FacilityType& item : other.facilitiesOptions) {
            facilitiesOptions.push_back(item);
        }
        for (BaseAction* item : other.actionsLog) {
            actionsLog.push_back(item->clone());
        }
        for (Settlement* item : other.settlements) {
            settlements.push_back(new Settlement(*item));
        }
        for (const Plan& item : other.plans) {
            Settlement& newSettlement = getSettlement(item.getSettlement().getName());
            plans.push_back(Plan(item, newSettlement, facilitiesOptions));
        }
    }
    return *this;
}

Simulation::Simulation(Simulation&& other)
    : isRunning(other.isRunning),
      planCounter(other.planCounter),
      actionsLog(std::move(other.actionsLog)),
      plans(std::move(other.plans)),
      settlements(std::move(other.settlements)),
      facilitiesOptions(std::move(other.facilitiesOptions)),
      unknownSettlement(other.unknownSettlement),
      unknownPlan(other.unknownPlan){
    other.unknownSettlement = nullptr;
    other.unknownPlan = nullptr;
}

Simulation& Simulation::operator=(Simulation&& other) {
    if (this != &other) {
        plans.clear();
        facilitiesOptions.clear();
        Clean();
        isRunning = other.isRunning;
        planCounter = other.planCounter;
        
        unknownSettlement = other.unknownSettlement;
        unknownPlan = other.unknownPlan;
        other.unknownSettlement = nullptr;
        other.unknownPlan = nullptr;
        
        facilitiesOptions = std::move(other.facilitiesOptions);
        actionsLog = std::move(other.actionsLog);
        settlements = std::move(other.settlements);
        plans = std::move(other.plans);
    }
    return *this;
}

void Simulation::Clean(){
    for (BaseAction* item : actionsLog) {
        delete item;
        item = nullptr;
    }
    for (Settlement* item : settlements) {
        delete item;
        item = nullptr;
    }
    actionsLog.clear();
    settlements.clear();
    delete unknownSettlement;
    delete unknownPlan;
    unknownSettlement = nullptr;
    unknownPlan = nullptr;
}

Simulation:: ~Simulation() {
    Clean();
}
