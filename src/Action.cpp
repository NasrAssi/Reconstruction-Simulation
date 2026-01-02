#include "Action.h"
#include "Simulation.h"
#include <iostream> // For cout, endl
using std::string;
using std::cout;
using std::endl;
SelectionPolicy* selectionPolicyFromString(const string& selectionPolicy);

//helper function
string statusToString(ActionStatus status){
    if (status == ActionStatus:: COMPLETED){
        return " COMPLETED";
    }
    if (status == ActionStatus:: ERROR){
        return " ERROR";
    }
    return "";
}

//BaseAction.
BaseAction:: BaseAction(): errorMsg(""), status(){}

ActionStatus BaseAction:: getStatus() const{
    return this->status;
}

void BaseAction:: complete() {
    this->status = ActionStatus:: COMPLETED;
}

void BaseAction:: error(string errorMsg) {
    this->status = ActionStatus:: ERROR;
    this-> errorMsg = errorMsg;
    cout << "Error: " << getErrorMsg() << endl;
}

const string& BaseAction:: getErrorMsg() const {
    return this->errorMsg;
}

//SimulateStep
SimulateStep::SimulateStep(const int numOfSteps): numOfSteps(numOfSteps){}

void SimulateStep::act(Simulation& simulation) {
    for (int i = 0; i < numOfSteps; i++) {
        simulation.step();
    }
    complete();
    simulation.addAction(this);
}
const string SimulateStep:: toString() const{
    return "step " + std::to_string(numOfSteps);
}
SimulateStep* SimulateStep::clone() const {
    return new SimulateStep(this->numOfSteps);
}

//AddPlan.
AddPlan:: AddPlan(const string& settlementName, const string& selectionPolicy)
: settlementName(settlementName), selectionPolicy(selectionPolicy){}

void AddPlan:: act(Simulation& simulation) {
    SelectionPolicy* selectedPolicy = selectionPolicyFromString(selectionPolicy);
    if (!simulation.isSettlementExists(settlementName) || selectedPolicy == nullptr){
        delete selectedPolicy;
        error("Cannot create this plan");
    }else {
        Settlement& Settlement = simulation.getSettlement(settlementName);
        simulation.addPlan(Settlement, selectedPolicy);
        complete();
    }
    simulation.addAction(this);
}

const string AddPlan:: toString() const{
    return "plan " + settlementName + " " + selectionPolicy;
}

AddPlan* AddPlan:: clone() const{
    return new AddPlan(settlementName,selectionPolicy);
}

//AddSettlement.
AddSettlement::AddSettlement(const string& settlementName,SettlementType settlementType)
:settlementName(settlementName),settlementType(settlementType){}

void AddSettlement:: act(Simulation& simulation){
    if (simulation.addSettlement(new Settlement(settlementName,settlementType)))
        complete();
    else
        error("Settlement already exists");
    simulation.addAction(this);
}
 
AddSettlement* AddSettlement:: clone() const{
    return new AddSettlement(settlementName,settlementType);
}

const string AddSettlement:: toString() const{
    return "settlement " +  settlementName + " " + std::to_string((int) settlementType);
}

//AddFacility.
AddFacility:: AddFacility(const string& facilityName, const FacilityCategory facilityCategory, const int price, const int lifeQualityScore, const int economyScore, const int environmentScore)
:facilityName(facilityName),facilityCategory(facilityCategory),price(price),lifeQualityScore(lifeQualityScore),economyScore(economyScore),environmentScore(environmentScore){}

void AddFacility:: act(Simulation &simulation){
    if (simulation.addFacility(std::move(FacilityType(facilityName,facilityCategory,price,lifeQualityScore,economyScore,environmentScore))))
        complete();
    else {
        error("Facility already exists");
    }
    simulation.addAction(this);
}

AddFacility* AddFacility:: clone() const {
    return new AddFacility(facilityName,facilityCategory,price,lifeQualityScore,economyScore,environmentScore);
}
 
const string AddFacility:: toString() const {
    return "facility " + facilityName + " " + std::to_string((int) facilityCategory) + " " + std::to_string(price) + " " + std::to_string(lifeQualityScore) + " " + std::to_string(economyScore) + " " + std::to_string(environmentScore);
}

//PrintPlanStatus.
PrintPlanStatus:: PrintPlanStatus(int planId): planId(planId) {}

void PrintPlanStatus:: act(Simulation &simulation) {
    if (simulation.isPlanExists(planId)){
        simulation.getPlan(planId).printStatus();
        complete();
    }else {
        error("Plan does not exist");
    }
    simulation.addAction(this);
}

PrintPlanStatus* PrintPlanStatus:: clone() const{
    return new PrintPlanStatus(planId);
}

const string PrintPlanStatus:: toString() const{
    return "planStatus " + std::to_string(planId);
}

//ChangePlanPolicy.
ChangePlanPolicy:: ChangePlanPolicy(const int planId, const string& newPolicy): planId(planId), newPolicy(newPolicy) {}

void ChangePlanPolicy:: act(Simulation& simulation) {
    if(!simulation.isPlanExists(planId)) {
        error("Plan does not exist");
        simulation.addAction(this);
        return;
    }
    string outPut = "planID: " + std::to_string(planId) + 
    "\n" + "previosPolicy: " + simulation.getPlan(planId).getSelectionPolicy()->toString() + 
    "\n" + "newPolicy: " + newPolicy;

    if (newPolicy != simulation.getPlan(planId).getSelectionPolicy()->toString()) {
        simulation.getPlan(planId).setSelectionPolicy(selectionPolicyFromString(newPolicy));
        complete();
        cout << outPut << endl;
    }else {
        error("Cannot change selection policy");
    }
    simulation.addAction(this);
}

ChangePlanPolicy* ChangePlanPolicy:: clone() const {
    return new ChangePlanPolicy(planId,newPolicy);
}

const string ChangePlanPolicy:: toString() const {
    return "changePolicy " + std::to_string(planId) + " " + newPolicy;
}

//PrintActionsLog.
void PrintActionsLog:: act(Simulation& simulation){
    for(BaseAction* action : simulation.GetActionsLog()){
        cout << action->toString() << statusToString(action->getStatus()) << endl;
    }
    simulation.addAction(this);
    complete();
}
 
PrintActionsLog* PrintActionsLog::clone() const {
    return new PrintActionsLog(*this);  
}

const string PrintActionsLog:: toString() const{
    return "log";
}

//close.
void Close:: act(Simulation& simulation){
    simulation.close();
    simulation.addAction(this);
    complete();
}
Close* Close::clone() const{
    return new Close(*this);
}
const string Close::toString() const{
    return "close";
}

//BackupSimulation.
void BackupSimulation::act(Simulation& simulation){
    if (backup != nullptr) {
        delete backup;
    }
    backup = new Simulation(simulation);
    complete();
    simulation.addAction(this);
}

BackupSimulation* BackupSimulation:: clone() const{
    return new BackupSimulation(*this);
}

const string BackupSimulation:: toString() const{
    return "backup";
}

//RestoreSimulation
void RestoreSimulation:: act(Simulation& simulation){
    if (backup == nullptr){
       error("no Back up avilibale");
    }else{
        simulation = *backup;
        complete();
    }
    
    simulation.addAction(this);
}

RestoreSimulation* RestoreSimulation:: clone() const{
    return new RestoreSimulation(*this);
}

const string RestoreSimulation::toString() const{
    return "restore";
}
