#include "Plan.h"

Plan::Plan(const int planId, const Settlement& settlement, SelectionPolicy* selectionPolicy, const vector<FacilityType>& facilityOptions)
: plan_id(planId), settlement(settlement), selectionPolicy(selectionPolicy),status(PlanStatus::AVALIABLE),facilities(),underConstruction(), facilityOptions(facilityOptions)
,life_quality_score(0),economy_score(0), environment_score(0){}

const int Plan:: getlifeQualityScore() const {
    return life_quality_score;
}

const int Plan:: getEconomyScore() const {
    return economy_score;
}

const int Plan:: getEnvironmentScore() const {
    return environment_score;
}

void Plan:: setSelectionPolicy(SelectionPolicy *selectionPolicy) {
    if(this->selectionPolicy != nullptr){
        delete this->selectionPolicy;
    }
    this->selectionPolicy = selectionPolicy;
}

void Plan::step(){

    if (this->status == PlanStatus::AVALIABLE) {
        while ((int)underConstruction.size() < this->settlement.getBuildCapacity()) {
            Facility* selectedFacility = new Facility(selectionPolicy-> selectFacility(facilityOptions), settlement.getName());
            this->addFacility(selectedFacility);
        }
    }
    
    for (size_t i=0; i < underConstruction.size(); i=i+1) {
        underConstruction[i]->step();
        if (underConstruction[i]-> getStatus() == FacilityStatus:: OPERATIONAL) {
            Facility* facility = underConstruction[i];
            facilities.push_back(facility);
            life_quality_score += facility-> getLifeQualityScore();
            economy_score += facility-> getEconomyScore();
            environment_score += facility-> getEnvironmentScore();
            underConstruction.erase(underConstruction.begin() + i);
            i--;
        }
    }

    if ((int)underConstruction.size() < this->settlement.getBuildCapacity()) {
        this-> status = PlanStatus:: AVALIABLE;
    }else {
        this-> status = PlanStatus:: BUSY;
    }
}

void Plan:: printStatus() const{
    cout << this-> toString() << endl;
    for (Facility* item: facilities) {
        cout << item->toString() << endl;
    }
    for (Facility* item: underConstruction) {
        cout << item->toString() << endl;
    }
}

const vector<Facility*>& Plan:: getFacilities() const{
    return facilities;
}


void Plan:: addFacility(Facility* facility){
    if (status == PlanStatus:: AVALIABLE) {
        underConstruction.push_back(facility);
    }else {
        delete facility;
    }
}

const string Plan:: toString() const{
    return "PlanID: " + std:: to_string(plan_id) +
    "\n" + "SettlementName: " + settlement.getName() +
    "\n" + "PlanStatus: " + statusToString() +
    "\n" + "SelectionPolicy: " + selectionPolicy-> toString() +
    "\n" + "LifeQualityScore: " + std:: to_string(life_quality_score) +
    "\n" + "EconomyScore: " + std:: to_string(economy_score) +
    "\n" + "EnvironmentScore: " + std:: to_string(environment_score);
}

Plan::Plan(const Plan& other, const Settlement& settlement, const vector<FacilityType>& facilityOptions)
    : plan_id(other.plan_id),
      settlement(settlement),
      selectionPolicy(other.selectionPolicy->clone()),
      status(other.status),
      facilities(),
      underConstruction(),
      facilityOptions(facilityOptions),
      life_quality_score(other.life_quality_score),
      economy_score(other.economy_score),
      environment_score(other.environment_score)
{
    for (Facility* item : other.facilities) {
        facilities.push_back(new Facility(*item));
    }
    for (Facility* item : other.underConstruction) {
        underConstruction.push_back(new Facility(*item));
    }
}

//rule of 5.
Plan:: Plan(const Plan& other): plan_id(other.plan_id), settlement(other.settlement), selectionPolicy(other.selectionPolicy->clone()), status(other.status),facilities(),underConstruction(), facilityOptions(other.facilityOptions), life_quality_score(other.life_quality_score), economy_score(other.economy_score), environment_score(other.environment_score) {
    for (Facility* item: other.facilities) {
        this->facilities.push_back(new Facility(*item));
    }
    for (Facility* item: other.underConstruction) {
        this->underConstruction.push_back(new Facility(*item));
    }
}

Plan:: Plan(Plan&& other): plan_id(other.plan_id),
    settlement(other.settlement),
    selectionPolicy(std::move(other.selectionPolicy)),status(other.status),
    facilities(std::move(other.facilities)),
    underConstruction(std::move(other.underConstruction)),facilityOptions(other.facilityOptions),
    life_quality_score(other.life_quality_score),
    economy_score(other.economy_score),
    environment_score(other.environment_score){
        other.selectionPolicy = nullptr;
    }


Plan:: ~Plan() {
    delete selectionPolicy;
    for (Facility* facility : underConstruction) {
        delete facility;
    }
    for (Facility* facility : facilities) {
        delete facility;
    }
    underConstruction.clear();
    facilities.clear();
}

//helper method.
const SelectionPolicy* Plan:: getSelectionPolicy() const{ 
    return this->selectionPolicy;
}
//helper method.
const Settlement& Plan::getSettlement() const{
    return settlement;
}
const string Plan::statusToString () const {
    if (status == PlanStatus:: AVALIABLE) {
        return "AVALIABLE";
    }
    if (status == PlanStatus:: BUSY) {
        return "BUSY";
    }
    return "UNKNOWN";
}
void Plan::printplan() const{
    cout << "PlanID: " + std:: to_string(plan_id) << endl;
    cout << "SettlementName: " << settlement.getName() << endl;
    cout << "LifeQualityScore: " << std:: to_string(life_quality_score) << endl;
    cout << "EconomyScore: " << std:: to_string(economy_score) << endl;
    cout << "EnvironmentScore: " << std:: to_string(environment_score) << endl;
}