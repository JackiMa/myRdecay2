#include "RunAction.hh"
#include "Run.hh"

#include "G4Run.hh"
#include "G4UnitsTable.hh"
#include "G4PhysicalConstants.hh"
#include "G4SystemOfUnits.hh"
#include <iomanip>

#include "G4AnalysisManager.hh"
#include "G4RunManager.hh"
#include "DetectorConstruction.hh"
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

RunAction::RunAction()
:G4UserRunAction(),
fRun(0)
{
  const DetectorConstruction *detectorConstruction = static_cast<const DetectorConstruction *>(G4RunManager::GetRunManager()->GetUserDetectorConstruction());
  CrystalName = detectorConstruction->CrystalName;
     // Create analysis manager
  // The choice of the output format is done via the specified
  // file extension.
  auto analysisManager = G4AnalysisManager::Instance();

  // Create directories
  //analysisManager->SetHistoDirectoryName("histograms");
  //analysisManager->SetNtupleDirectoryName("ntuple");
  analysisManager->SetVerboseLevel(1);
  analysisManager->SetNtupleMerging(true);
    // Note: merging ntuples is available only with Root output

  G4cout << "\n--------------- create H2 ---------------\n";
  analysisManager->CreateH2("Time_Energy",this->CrystalName,300,1*s,5000*s,300,0,3*MeV,"s","keV");
  G4cout << "--------------- create H2 ---------------\\nn";
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

RunAction::~RunAction()
{}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4Run* RunAction::GenerateRun()
{ 
  fRun = new Run();
  return fRun;
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void RunAction::BeginOfRunAction(const G4Run*)
{

    // 尝试使用g4root来画图
  // Get analysis manager
  auto analysisManager = G4AnalysisManager::Instance();

  // Open an output file
  //
  G4String fileName = this->CrystalName + ".root";
  // Other supported output types:
  // G4String fileName = "B4.csv";
  analysisManager->OpenFile(fileName);
  G4cout << "Using " << analysisManager->GetType() << G4endl;
  // g4root 👆

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void RunAction::EndOfRunAction(const G4Run*)
{
 if (isMaster) {
  fRun->EndOfRun();
 }


  // g4root
  auto analysisManager = G4AnalysisManager::Instance();
  analysisManager->WriteH2(0,this->CrystalName+"_TimevsEng.csv"); // 将 Histgoram 输出成 csv

  analysisManager->Write();
  analysisManager->CloseFile();


}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
