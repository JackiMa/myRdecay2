#include "SteppingAction.hh"
#include "EventAction.hh"
#include "RunAction.hh"
#include "Run.hh"

#include "G4Step.hh"
#include "G4Event.hh"
#include "G4RunManager.hh"

#include "G4AnalysisManager.hh"
#include "G4SystemOfUnits.hh"

#include <fstream>

#define OUTFILENAME "./output/data.csv"

SteppingAction::SteppingAction(EventAction *event, RunAction *runaction)
    : G4UserSteppingAction(), fEvent(event), fRunAction(runaction)
{
}

SteppingAction::~SteppingAction()
{
}

void SteppingAction::UserSteppingAction(const G4Step *aStep)
{
    // G4Track *theTrack = aStep->GetTrack();                                         // 获取径迹
    // G4String particlename = aStep->GetTrack()->GetDefinition()->GetParticleName(); // 获取粒子名称
    // G4int StepID = theTrack->GetCurrentStepNumber();                               // 获取当前stepID
    // G4int ParentID = theTrack->GetParentID();                                      // 获取母核的ID
    // G4int EventID = fEvent->EventID;
    // G4int TrackID = theTrack->GetTrackID();

    // const G4StepPoint *fpPreStepPoint = aStep->GetPreStepPoint();
    // const G4StepPoint *fpPostStepPoint = aStep->GetPostStepPoint();

    // // fill ntuple
    // analysisManager->FillNtupleDColumn(0, TrackID);
    // analysisManager->FillNtupleDColumn(1, theTrackTime);
    // analysisManager->FillNtupleDColumn(2, fEnergyTrack);
    // analysisManager->AddNtupleRow();

    // std::fstream datafile;
    // datafile.open("outputEnergyTest.csv", std::ios::out | std::ios::app);
    // datafile << TrackID << "\t" << StepID << "\t" <<aStep->GetDeltaEnergy() << "\t"<<fEnergyTrack<<"\t"<<theTrackTime<<"\t"<<aStep->GetSecondary()  << "\n";
    // datafile.close();

    const G4StepPoint *fpPostStepPoint = aStep->GetPostStepPoint();
    // G4cout << "fpPostStepPoint->GetMaterial()->GetName() = "<<fpPostStepPoint->GetMaterial()->GetName()<<"\tfRun->CrystalName = "<<fRun->CrystalName<<"\n";
    // const G4StepPoint *fpPreStepPoint = aStep->GetPreStepPoint();

    if (fpPostStepPoint->GetStepStatus() != fWorldBoundary)
    {
        // 如果没有射出世界体，才做进一步判断，不然会报错
        G4double globalTime = fpPostStepPoint->GetGlobalTime();
        // G4cout << "globalTime = " << globalTime << "\tMaterial = " << fpPostStepPoint->GetMaterial()->GetName() << "\n";
        // 只考虑1s到1天之间的数据
        if ((globalTime > 1 * s) && (globalTime < 1 * day) &&(fpPostStepPoint->GetMaterial()->GetName() == fRunAction->CrystalName))
        {
            G4double totalDep = aStep->GetTotalEnergyDeposit()/keV; // 以keV为默认单位
            if(totalDep > 0){ // 不考虑沉积能量太小太小的
                // std::fstream coinFileIon;                              // 输出在晶体内沉积能量的大小和时间
                // coinFileIon.open("CoinCrystalDep_"+fRun->CrystalName+".csv", std::ios::app); // ios::app 在文件尾追加输入
                // coinFileIon << globalTime/s << "," << totalDep << "\n"; 
                // coinFileIon.close();
                  Run* run = static_cast<Run*>(G4RunManager::GetRunManager()->GetNonConstCurrentRun());
         
                run->CoinIonCount(globalTime,totalDep );
            }

        }
    }
}