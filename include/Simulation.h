#pragma once
#include <string>
#include <vector>
#include "Facility.h"
#include "Plan.h"
#include "Settlement.h"
#include "Auxiliary.h"
using std::string;
using std::vector;


class BaseAction;
class SelectionPolicy;

class Simulation {
    public:
        Simulation(const string& configFilePath);
        void start();
        void addPlan(const Settlement& settlement, SelectionPolicy* selectionPolicy);
        void addAction(BaseAction* action);
        bool addSettlement(Settlement* settlement);
        bool addFacility(FacilityType facility);
        bool isSettlementExists(const string& settlementName) const;
        Settlement& getSettlement(const string& settlementName);
        Plan& getPlan(int planID);
        void step();
        void close();
        void open();
        //helper methods.
        const vector<BaseAction*>& GetActionsLog() const;
        const bool isPlanExists(int planId) const;
        //rule of 5.
        Simulation(const Simulation& other);
        Simulation& operator= (const Simulation& other);
        Simulation(Simulation&& other);
        Simulation& operator = (Simulation &&other);
        ~Simulation();

    private:
        bool isRunning;
        int planCounter; //For assigning unique plan IDs
        vector<BaseAction*> actionsLog;
        vector<Plan> plans;
        vector<Settlement*> settlements;
        vector<FacilityType> facilitiesOptions;
        Settlement* unknownSettlement; //does not exsit.
        Plan* unknownPlan; //does not exsit.
        void Clean(); //helper method
};
