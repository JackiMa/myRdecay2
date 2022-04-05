#ifndef TrackingAction_h
#define TrackingAction_h 1

#include "G4UserTrackingAction.hh"
#include "globals.hh"

class RunAction;
class EventAction;
class TrackingMessenger;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class TrackingAction : public G4UserTrackingAction {

  public:  
    TrackingAction(EventAction *event, RunAction *run);
   ~TrackingAction();
   
    virtual void  PreUserTrackingAction(const G4Track*);
    virtual void PostUserTrackingAction(const G4Track*);

  private:
    EventAction*  fEvent;
    RunAction*  fRun;

    G4double fCharge, fMass;        

};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
