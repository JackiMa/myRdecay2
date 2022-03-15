
#ifndef SteppingAction_h
#define SteppingAction_h 1

#include "G4UserSteppingAction.hh"
#include "globals.hh"

class EventAction;
class RunAction;

class G4LogicalVolume;

/// Stepping action class
/// 

class SteppingAction : public G4UserSteppingAction
{
  public:
    SteppingAction(EventAction* event,RunAction*runAction);
    virtual ~SteppingAction();

    // method from the base class
    virtual void UserSteppingAction(const G4Step*);

  private:
    EventAction*  fEvent;
    RunAction*  fRun;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
