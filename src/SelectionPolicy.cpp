#include "SelectionPolicy.h"
#include "Facility.h"
#include <limits.h>
#include <algorithm>
using namespace std;
const string statusToString(FacilityStatus status);//helper function

NaiveSelection::NaiveSelection() : lastSelectedIndex(-1) {};

const FacilityType& NaiveSelection::selectFacility(const vector<FacilityType>& facilitiesOptions){
    lastSelectedIndex = (lastSelectedIndex + 1) % facilitiesOptions.size();
    return facilitiesOptions[lastSelectedIndex];
}

const string NaiveSelection::toString() const{
    return "nve";
}

NaiveSelection* NaiveSelection::clone() const{
    NaiveSelection* outPut = new NaiveSelection();
    outPut->lastSelectedIndex = lastSelectedIndex;
    return outPut;
}


BalancedSelection:: BalancedSelection(int LifeQualityScore, int EconomyScore, int EnvironmentScore) : LifeQualityScore(LifeQualityScore), EconomyScore(EconomyScore), EnvironmentScore(EnvironmentScore) {};

const FacilityType& BalancedSelection::selectFacility(const vector<FacilityType>& facilitiesOptions){
    int minDistance(INT_MAX);
    int distance;
    const FacilityType* outPut = nullptr;
    int totalLifeQualityScore;
    int totalEconomyScore;
    int totalEnvironmentScore;
    for (const FacilityType& item : facilitiesOptions){
        totalLifeQualityScore = item.getLifeQualityScore() + LifeQualityScore;
        totalEconomyScore = item.getEconomyScore() + EconomyScore;
        totalEnvironmentScore = item.getEnvironmentScore() + EnvironmentScore;
        distance = std::max({totalLifeQualityScore, totalEconomyScore, totalEnvironmentScore}) - std::min({totalLifeQualityScore, totalEconomyScore, totalEnvironmentScore});
        if (distance < minDistance){
            minDistance = distance;
            outPut = &item;
        }
    }
    if(outPut != nullptr){
        LifeQualityScore += outPut->getLifeQualityScore();
        EconomyScore += outPut->getEconomyScore();
        EnvironmentScore += outPut->getEnvironmentScore();
    }
    return *outPut;
}

const string BalancedSelection::toString() const{
    return "bal";
}

BalancedSelection *BalancedSelection::clone() const{
    return new BalancedSelection(LifeQualityScore, EconomyScore, EnvironmentScore);
}


EconomySelection::EconomySelection() : lastSelectedIndex(-1) {};

const FacilityType& EconomySelection::selectFacility(const vector<FacilityType>& facilitiesOptions){
    while (true){
        lastSelectedIndex = (lastSelectedIndex + 1) % facilitiesOptions.size();
        if (facilitiesOptions[lastSelectedIndex].getCategory() == FacilityCategory:: ECONOMY){
            return facilitiesOptions[lastSelectedIndex];
        }
    }
}

const string EconomySelection::toString() const{
    return "eco";
}

EconomySelection* EconomySelection::clone() const{
    EconomySelection* outPut = new EconomySelection();
    outPut->lastSelectedIndex = lastSelectedIndex;

    return outPut;
}


SustainabilitySelection::SustainabilitySelection() : lastSelectedIndex(-1) {}

const FacilityType& SustainabilitySelection::selectFacility(const vector<FacilityType>& facilitiesOptions){
    while (true){
        lastSelectedIndex = (lastSelectedIndex + 1) % facilitiesOptions.size();
        if (facilitiesOptions[lastSelectedIndex].getCategory() == FacilityCategory::ENVIRONMENT){
            return facilitiesOptions[lastSelectedIndex];
        }
    }
}

const string SustainabilitySelection::toString() const{
    return "env";
}

SustainabilitySelection* SustainabilitySelection::clone() const{
    SustainabilitySelection* outPut = new SustainabilitySelection();
    outPut->lastSelectedIndex = lastSelectedIndex;
    return outPut;
}
