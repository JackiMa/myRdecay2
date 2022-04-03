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

    // std::fstream coinFile; // 输出在晶体内沉积能量的大小和时间




    
}