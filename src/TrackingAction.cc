#include "TrackingAction.hh"

#include "Run.hh"
#include "EventAction.hh"
#include "RunAction.hh"

#include "G4Track.hh"
#include "G4ParticleTypes.hh"
#include "G4IonTable.hh"
#include "G4RunManager.hh"

#include "G4SystemOfUnits.hh"
#include "G4UnitsTable.hh"

#include <fstream>
#include "G4AnalysisManager.hh"
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

TrackingAction::TrackingAction(EventAction *event, RunAction *run)
:G4UserTrackingAction(),
fEvent(event), fRun(run)
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
    // 只考虑1s~1day的数据
  if((track->GetDefinition()->GetParticleName() == "gamma") && (globalTime > 1*s)&& (globalTime < 1*day)){
      analysisManager->FillH2(0,globalTime,fEnergyTrack);
      
      const G4VProcess *createdProcess = track->GetCreatorProcess();
      G4String createProcessName; // 产生这次事件的物理过程
      if(createdProcess == nullptr){
        createProcessName = "particle gun";
      }
      else{
        createProcessName = createdProcess->GetProcessName();
      }
      std::fstream coinFileGamma; // 输出在晶体内沉积能量的大小和时间
      coinFileGamma.open("CoinOutGamma_"+fRun->CrystalName+".csv",std::ios::app); // ios::app 在文件尾追加输入
      coinFileGamma <<std::setprecision(15) << globalTime/s<< "," << fEnergyTrack/keV <<","<< createProcessName<<"\n"; // 时间以s为单位
      coinFileGamma.close();

  }
}

  

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

