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


#define OUTFILENAME "./output/data.csv"

// ==1 表示每次run之后清理nuclideTable，==0表示累加计数
// 对于gps来说，一次run是一个能谱，每次结果需要清零。而对于gun来说，更适合累加
#define isCleanAfterRun 1 

std::map<G4String, G4int> RunAction::nuclideTable;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

RunAction::RunAction()
:G4UserRunAction()
{}

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
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
