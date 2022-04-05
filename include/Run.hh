#ifndef Run_h
#define Run_h 1

#include "G4Run.hh"
#include "G4VProcess.hh"
#include "globals.hh"
#include <map>

class G4ParticleDefinition;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class Run : public G4Run
{
  public:
    Run();
   ~Run();

  public:
    void ParticleCount(G4String, G4double, G4double);
    void CoinIonCount(G4double, G4double); // 统计沉积能量的粒子

    // void SetPrimary(G4ParticleDefinition* particle, G4double energy);
    void EndOfRun(); 

    virtual void Merge(const G4Run*);

  private:    
    struct ParticleData {
     ParticleData()
       : fCount(0), fEmean(0.), fEmin(0.), fEmax(0.), fTmean(-1.) {}
     ParticleData(G4int count, G4double ekin, G4double emin, G4double emax,
                  G4double meanLife)
       : fCount(count), fEmean(ekin), fEmin(emin), fEmax(emax),
         fTmean(meanLife) {}
     G4int     fCount;
     G4double  fEmean;
     G4double  fEmin;
     G4double  fEmax;
     G4double  fTmean;
    };
     
  private: 
    // G4ParticleDefinition*  fParticle;
    // G4double  fEkin;
             
    std::map<G4String,ParticleData>  fParticleDataMap;    
       

private:    
  struct ActivityData {
   ActivityData()
     : fNlife_t1(0), fNlife_t2(0), fNdecay_t1t2(0) {}
   ActivityData(G4int n1, G4int n2, G4int nd)
     : fNlife_t1(n1), fNlife_t2(n2), fNdecay_t1t2(nd) {}
   G4int  fNlife_t1;
   G4int  fNlife_t2;
   G4int  fNdecay_t1t2;
  };
  
  std::map<G4String,ActivityData>  fActivityMap;

  private:
  std::map<G4double,G4double>  coinIonDataMap; 
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif

