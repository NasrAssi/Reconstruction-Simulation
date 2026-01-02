#pragma once
#include <vector>
#include "Facility.h"
#include "Settlement.h"
#include "SelectionPolicy.h"
#include <iostream>
using namespace std;
using std::vector;

enum class PlanStatus {
    AVALIABLE,
    BUSY,
};

class Plan {
    public:
        Plan(const int planId, const Settlement& settlement, SelectionPolicy* selectionPolicy, const vector<FacilityType>& facilityOptions);
        const int getlifeQualityScore() const;
        const int getEconomyScore() const;
        const int getEnvironmentScore() const;
        void setSelectionPolicy(SelectionPolicy* selectionPolicy);
        void step();
        void printStatus() const;
        const vector<Facility*>& getFacilities() const;
        void addFacility(Facility* facility);
        const string toString() const;
        const SelectionPolicy* getSelectionPolicy() const; //helper method.
        const string statusToString() const; //helper method
        void printplan() const;
        //helper constructor for simulation copy.
        Plan(const Plan& other, const Settlement& settlement, const vector<FacilityType>& facilityOptions);
        // rule of 5.
        Plan(const Plan& other);
        Plan& operator = (const Plan& other) = delete;
        Plan(Plan&& other);
        Plan& operator = (const Plan&& other) = delete;
        ~Plan();
        const Settlement& getSettlement() const;


    private:
        int plan_id;
        const Settlement& settlement;
        SelectionPolicy* selectionPolicy;
        PlanStatus status;
        vector<Facility*> facilities;
        vector<Facility*> underConstruction;
        const vector<FacilityType>& facilityOptions;
        int life_quality_score, economy_score, environment_score;
};