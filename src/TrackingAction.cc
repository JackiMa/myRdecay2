#include "TrackingAction.hh"

#include "Run.hh"
#include "EventAction.hh"

#include "G4Track.hh"
#include "G4ParticleTypes.hh"
#include "G4IonTable.hh"
#include "G4RunManager.hh"

#include "G4SystemOfUnits.hh"
#include "G4UnitsTable.hh"

#include <fstream>
#include "G4AnalysisManager.hh"
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

TrackingAction::TrackingAction(EventAction* fEventAction)
:G4UserTrackingAction(),
 fEvent(fEventAction) 
{

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

TrackingAction::~TrackingAction()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void TrackingAction::PreUserTrackingAction(const G4Track* track)
{
  Run* run 
   = static_cast<Run*>(G4RunManager::GetRunManager()->GetNonConstCurrentRun());
         
  G4ParticleDefinition* particle = track->GetDefinition();
  G4String name   = particle->GetParticleName();
  fCharge = particle->GetPDGCharge();
  fMass   = particle->GetPDGMass();  
    
  G4double Ekin = track->GetKineticEnergy();
  G4int ID      = track->GetTrackID();

  G4bool condition = false;
  
  // check LifeTime
  //
  G4double meanLife = particle->GetPDGLifeTime();
  
  //count particles
  //
  run->ParticleCount(name, Ekin, meanLife);
  
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void TrackingAction::PostUserTrackingAction(const G4Track* track)
{

G4double globalTime = track->GetGlobalTime();

    G4double fEnergyTrack = track->GetTotalEnergy();
    auto analysisManager = G4AnalysisManager::Instance();
    // std::cout << "the Track Time is " << globalTime << " ns\n";


if(track->GetStep()->GetPostStepPoint()->GetStepStatus() == fWorldBoundary){
    // 只考虑跑出世界体的gamma
  if(track->GetDefinition()->GetParticleName() == "gamma"){
      analysisManager->FillH2(0,globalTime,fEnergyTrack);
  }
}

  

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

