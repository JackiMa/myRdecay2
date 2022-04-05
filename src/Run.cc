#include "Run.hh"
#include "G4RunManager.hh"
#include "PrimaryGeneratorAction.hh"

#include "G4SystemOfUnits.hh"
#include "G4UnitsTable.hh"
#include "G4PhysicalConstants.hh"

#include "DetectorConstruction.hh"
#include <fstream>
#define OUTPATH "./output/"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

Run::Run()
: G4Run()
{}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

Run::~Run()
{ }

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void Run::CoinIonCount(G4double globalTime, G4double depInCrystal){

globalTime = (G4long) globalTime / (0.1*us); // 设可以分辨的最小时间为0.1us，那么按此来合并.
                                             // 通过数制转换来强行实现数据的合并

  // 合并同一时间沉积的能量
  std::map<G4double, G4double>::iterator it = coinIonDataMap.find(globalTime);
  if ( it == coinIonDataMap.end()) {
    coinIonDataMap[globalTime] = depInCrystal;
  }
  else{
    coinIonDataMap[globalTime] += depInCrystal;
  }
}

void Run::ParticleCount(G4String name, G4double Ekin, G4double meanLife)
{
  std::map<G4String, ParticleData>::iterator it = fParticleDataMap.find(name);
  if ( it == fParticleDataMap.end()) {
    fParticleDataMap[name] = ParticleData(1, Ekin, Ekin, Ekin, meanLife);
  }
  else {
    ParticleData& data = it->second;
    data.fCount++;
    data.fEmean += Ekin;
    //update min max
    G4double emin = data.fEmin;
    if (Ekin < emin) data.fEmin = Ekin;
    G4double emax = data.fEmax;
    if (Ekin > emax) data.fEmax = Ekin;
    data.fTmean = meanLife;
  }   
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void Run::Merge(const G4Run* run)
{
  const Run* localRun = static_cast<const Run*>(run);

  //primary particle info
  //
  // fParticle = localRun->fParticle;
  // fEkin     = localRun->fEkin;

  // fParticleDataMap
  std::map<G4String,ParticleData>::const_iterator itn;
  for (itn = localRun->fParticleDataMap.begin(); 
       itn != localRun->fParticleDataMap.end(); ++itn) {
    
    G4String name = itn->first;
    const ParticleData& localData = itn->second;   
    if ( fParticleDataMap.find(name) == fParticleDataMap.end()) {
      fParticleDataMap[name]
       = ParticleData(localData.fCount, 
                      localData.fEmean, 
                      localData.fEmin, 
                      localData.fEmax,
                      localData.fTmean);
    }
    else {
      ParticleData& data = fParticleDataMap[name];   
      data.fCount += localData.fCount;
      data.fEmean += localData.fEmean;
      G4double emin = localData.fEmin;
      if (emin < data.fEmin) data.fEmin = emin;
      G4double emax = localData.fEmax;
      if (emax > data.fEmax) data.fEmax = emax;
      data.fTmean = localData.fTmean;
    }   
  }

  // coinIonDataMap
  std::map<G4double, G4double>::const_iterator itn2;
  for (itn2 = localRun->coinIonDataMap.begin(); 
       itn2 != localRun->coinIonDataMap.end(); ++itn2) {
    
    G4double globalTime = itn2->first;
    const G4double localData = itn2->second;   
    if ( coinIonDataMap.find(globalTime) == coinIonDataMap.end()) {
      coinIonDataMap[globalTime] = localData;
    }
    else {
        coinIonDataMap[globalTime] += localData;
    }   
  }

  G4Run::Merge(run); 
} 
    
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void Run::EndOfRun() 
{

  std::string pathname = OUTPATH;
  try{
        system(("mkdir " +  pathname).c_str()); // 创建文件夹
    }catch(const char* &e){
    }
  const DetectorConstruction *detectorConstruction = static_cast<const DetectorConstruction *>(G4RunManager::GetRunManager()->GetUserDetectorConstruction());
  G4String CrystalName = detectorConstruction->CrystalName;
  runID = this->GetRunID();

 G4int nbEvents = numberOfEvent;
//  G4String partName = fParticle->GetParticleName();
 
 G4cout << "\n ======================== run summary ======================";  
//  G4cout << "\n The run was " << nbEvents << " " << partName << " of "
//         << G4BestUnit(fEkin,"Energy");
 G4cout << "\n ===========================================================\n";
 G4cout << G4endl;
 if (nbEvents == 0) { return; }
 
 G4int prec = 4, wid = prec + 2;
 G4int dfprec = G4cout.precision(prec);
      
 //particle count
 //
 G4cout << " Nb of generated particles: \n" << G4endl;

  std::ofstream fr;
  fr.open((OUTPATH+CrystalName+".csv").c_str(),std::ios::app); // ios::app 在文件尾追加输入
  fr << "-------------------- RunID = " << runID << "-------------------- \n";
  fr << "Particle\tNums\tEmean\tEmin\tEmax\tstatus"<<std::endl;
  
  std::ofstream frSimple;
  frSimple.open((OUTPATH+CrystalName+"_Simple.csv").c_str(),std::ios::app); // ios::app 在文件尾追加输入

 std::map<G4String,ParticleData>::iterator it;               
 for (it = fParticleDataMap.begin(); it != fParticleDataMap.end(); it++) { 
    G4String name     = it->first;
    ParticleData data = it->second;
    G4int count    = data.fCount;
    G4double eMean = data.fEmean/count;
    G4double eMin  = data.fEmin;
    G4double eMax  = data.fEmax;
    G4double meanLife = data.fTmean;
         
    G4cout << "  " << std::setw(15) << name << ": " << std::setw(7) << count
           << "  Emean = " << std::setw(wid) << G4BestUnit(eMean, "Energy")
           << "\t( "  << G4BestUnit(eMin, "Energy")
           << " --> " << G4BestUnit(eMax, "Energy") << ")";
    if (meanLife > 0.)
      G4cout << "\tmean life = " << G4BestUnit(meanLife, "Time")   << G4endl;
    else if (meanLife < 0.) G4cout << "\tstable" << G4endl;

    // 写入文件
    fr << name << "\t" << count
           << "\t" << G4BestUnit(eMean, "Energy")
           << "\t"  << G4BestUnit(eMin, "Energy")
           << "\t" << G4BestUnit(eMax, "Energy") ;
    if (meanLife > 0.)
      fr << "\t" << G4BestUnit(meanLife, "Time")   << G4endl;
    else if (meanLife < 0.) fr << "\tstable" << G4endl;
    else fr << G4endl;
    // --
    frSimple << name << "\t" << count << G4endl;

    
 }
 fr.close();
 frSimple.close();


  std::fstream coinFileIon;                              // 输出在晶体内沉积能量的大小和时间
  coinFileIon.open("CoinCrystalDep_"+CrystalName+".csv", std::ios::app); // ios::app 在文件尾追加输入

   std::map<G4double, G4double>::const_iterator it2;               
 for (it2 = coinIonDataMap.begin(); it2 != coinIonDataMap.end(); it2++) { 
    G4double globalTime     = it2->first;
    G4double edepInCrystal = it2->second;

  coinFileIon << std::setprecision(15) << globalTime *us / s << "," << edepInCrystal/keV; // 将单位换算到s和keV
  coinFileIon << "\n";
    
 }
  coinFileIon.close();
 
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
