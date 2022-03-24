#include "SteppingAction.hh"
#include "EventAction.hh"
#include "RunAction.hh"

#include "G4Step.hh"
#include "G4Event.hh"
#include "G4RunManager.hh"

#include "G4AnalysisManager.hh"
#include "G4SystemOfUnits.hh"

#include <fstream>

#define OUTFILENAME "./output/data.csv"

SteppingAction::SteppingAction(EventAction *event, RunAction *run)
    : G4UserSteppingAction(), fEvent(event), fRun(run)
{
}

SteppingAction::~SteppingAction()
{
}

void SteppingAction::UserSteppingAction(const G4Step *aStep)
{
    G4Track *theTrack = aStep->GetTrack();                                         // 获取径迹
    G4String particlename = aStep->GetTrack()->GetDefinition()->GetParticleName(); // 获取粒子名称
    G4int StepID = theTrack->GetCurrentStepNumber();                               // 获取当前stepID
    G4int ParentID = theTrack->GetParentID();                                      // 获取母核的ID
    G4int EventID = fEvent->EventID;
    G4int TrackID = theTrack->GetTrackID();

    // const G4StepPoint *fpPreStepPoint = aStep->GetPreStepPoint();
    // const G4StepPoint *fpPostStepPoint = aStep->GetPostStepPoint();

    // const G4VProcess *StepProcess = aStep->GetPostStepPoint()->GetProcessDefinedStep(); // 获取此step的物理过程
    // G4String StepProcessName = StepProcess->GetProcessName();

    // G4double En = fpPreStepPoint->GetKineticEnergy(); // 当前作用前的粒子动能
    // G4double EnT = fpPreStepPoint->GetTotalEnergy();  // 当前track的总能量

    // G4String postVolume = "NULL";
    // G4String preVolume = "NULL_pre";
    // if (theTrack->GetNextVolume()) // 判断粒子是否在世界中
    // {
    //     postVolume = fpPostStepPoint->GetPhysicalVolume()->GetName(); // 获取粒子所在体的名称，用于判断粒子位置
    //     preVolume = fpPreStepPoint->GetPhysicalVolume()->GetName();
    // }

    // // ..........................................
    // if (particlename) // == "neutron" && StepID == 1) // StepID == 1 表示刚产生该粒子的径迹
    // {
    //     std::fstream datafile;
    //     datafile.open(OUTFILENAME, std::ios::out | std::ios::app);
    //     datafile << EventID << "\t" << ParentID << "\t" << TrackID << "\t" << StepID << "\t" << particlename << "\t" << En << "\t" << EnT << "\t" << StepProcessName << "\n";
    //     datafile.close();
    // }
    // if (StepID == 1)
    // { // 确保只记录一遍
    //     // 建立一个表来记录出现这些粒子的次数

    //     std::map<G4String, G4int>::iterator iter = fRun->nuclideTable.find(particlename);
    //     if (iter != fRun->nuclideTable.end())
    //     {
    //         iter->second++;
    //     }
    //     else // 如果找不到就添加一个，找到了就count++
    //     {
    //         fRun->nuclideTable.insert(std::pair<G4String, G4int>(particlename, 1)); // 如果测试用例为 "qwerqwer"时，前4次循环都是执行的这句else即insert插入操作
    //     }
    //     // G4cout << "找到了 " << iter->second <<" 个 "<< particlename << "n";


            


    // }

    G4double theTrackTime = theTrack->GetGlobalTime();
    G4double fEnergyTrack = theTrack->GetTotalEnergy();
    auto analysisManager = G4AnalysisManager::Instance();
    // std::cout << "the Track Time is " << theTrackTime << " ns\n";

if(theTrackTime < 1){

}else if (theTrackTime < 1*us){
    analysisManager->FillH1(1, fEnergyTrack);
}else if (theTrackTime < 1*ms){
    analysisManager->FillH1(2, fEnergyTrack);
}else if (theTrackTime < 1*s){
    analysisManager->FillH1(3, fEnergyTrack);
}else if (theTrackTime < 1000*s){
    analysisManager->FillH1(4, fEnergyTrack);
}else{
    analysisManager->FillH1(5, fEnergyTrack);
}

        // // fill ntuple
        // analysisManager->FillNtupleDColumn(0, TrackID);
        // analysisManager->FillNtupleDColumn(1, theTrackTime);
        // analysisManager->FillNtupleDColumn(2, fEnergyTrack);
        // analysisManager->AddNtupleRow();

        // std::fstream datafile;
        // datafile.open("outputEnergyTest.csv", std::ios::out | std::ios::app);
        // datafile << TrackID << ", "<<theTrackTime << ", " << fEnergyTrack << "\n";
        // datafile.close();





    
}