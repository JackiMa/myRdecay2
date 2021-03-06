#include "RunAction.hh"
#include "PrimaryGeneratorAction.hh"
#include "DetectorConstruction.hh"

#include "G4Run.hh"
#include "G4RunManager.hh"
#include "G4UnitsTable.hh"
#include "G4PhysicalConstants.hh"
#include "G4SystemOfUnits.hh"
#include <iomanip>
#include <fstream>

#include "G4AnalysisManager.hh"

#define OUTFILENAME "./output/data.csv"

// ==1 表示每次run之后清理nuclideTable，==0表示累加计数
// 对于gps来说，一次run是一个能谱，每次结果需要清零。而对于gun来说，更适合累加
#define isCleanAfterRun 1 

std::map<G4String, G4int> RunAction::nuclideTable;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

RunAction::RunAction()
:G4UserRunAction()
{
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

  // Book histograms, ntuple
  //
  // Creating histograms
    analysisManager->SetH1XAxisTitle(2,"SetH1XAxisTitle");

  analysisManager->CreateH1("E_radiation","Time = 1ns", 1024, 0., 3*MeV,"MeV");
  // analysisManager->CreateH1("E_radiation","Time = 1ns", 1024, 0., 3*MeV); // 占位
  analysisManager->CreateH1("E_radiation","Time = 1s", 1024, 0, 1*MeV,"MeV");
  analysisManager->CreateH1("E_radiation","Time = 10s", 1024, 0., 3*MeV,"MeV");
  analysisManager->CreateH1("E_radiation","Time = 100s", 1024, 0., 3*MeV,"MeV");
  analysisManager->CreateH1("E_radiation","Time = 1ks", 1024, 0., 3*MeV,"MeV");
  analysisManager->CreateH1("E_radiation","Time = 1ks+", 1024, 0., 3*MeV,"MeV");
  



  // // Creating ntuple
  // //
  // analysisManager->CreateNtuple("Radiation", "Edep");
  // analysisManager->CreateNtupleDColumn("TrackID");
  // analysisManager->CreateNtupleDColumn("GlobalTime");
  // analysisManager->CreateNtupleDColumn("E_radiation");
  // analysisManager->FinishNtuple();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

RunAction::~RunAction()
{}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void RunAction::BeginOfRunAction(const G4Run* aRun)
{ 



if(IsMaster()){
  G4cout << " ### Run "<< aRun->GetRunID() << " start." << G4endl;
  std::ofstream fr;
  fr.open(OUTFILENAME,std::ios::app); // ios::app 在文件尾追加输入
  fr << "————————————————  Run ID = "<<aRun->GetRunID() <<"  ————————————————"<<std::endl;
  fr << "EventID\tParent\tTrack\tStep\tParticle\tEnergy\tTotalEnergy\tProcess"<<std::endl;
  fr.close();

if(isCleanAfterRun){
  // 清空列表，如果不清空的话，多次run的结果会累加
  this->nuclideTable.erase(this->nuclideTable.begin(),this->nuclideTable.end()); 
}



}

  // 尝试使用g4root来画图
  // Get analysis manager
  auto analysisManager = G4AnalysisManager::Instance();

  // Open an output file
  //
  const DetectorConstruction *detectorConstruction = static_cast<const DetectorConstruction *>(G4RunManager::GetRunManager()->GetUserDetectorConstruction());
  G4String CrystalName = detectorConstruction->CrystalName;
  G4String fileName = CrystalName + ".root";
  // Other supported output types:
  // G4String fileName = "B4.csv";
  analysisManager->OpenFile(fileName);
  G4cout << "Using " << analysisManager->GetType() << G4endl;
  // g4root 👆
  

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void RunAction::EndOfRunAction(const G4Run* aRun)
{

  if(IsMaster()){
    G4cout << "\n*******************************************"<< G4endl;
    G4cout << "\t\tNumber of event = "<< aRun->GetNumberOfEvent() << G4endl;
    G4cout << "*******************************************\n"<< G4endl;

    std::map<G4String, G4int>::iterator iter = this->nuclideTable.begin();

    G4cout << "开始输出记录的结果：" ;
    for (; iter != this->nuclideTable.end(); iter++)
    {
        G4cout << iter->first << '\t' << iter->second << G4endl;
    }
    G4cout << G4endl;
    G4cout << "cout完毕!\n" ;
  }

        // g4root
  auto analysisManager = G4AnalysisManager::Instance();
    // save histograms & ntuple


  G4cout << "------------------------------------" << "\n";
  G4cout << "------------------------------------" << "\n";
  G4cout << "------------------------------------" << "\n";
  G4cout << "------------------------------------" << "\n";
  G4cout << analysisManager->GetH1Nbins(0) << "\n";
  G4cout << analysisManager->WriteH1(1,"testGAGG.csv") << "\n";
  G4cout << "------------------------------------" << "\n";
  G4cout << "------------------------------------" << "\n";
  G4cout << "------------------------------------" << "\n";
  G4cout << "------------------------------------" << "\n";


  analysisManager->Write();
  analysisManager->CloseFile();
  // --------------------------

}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
