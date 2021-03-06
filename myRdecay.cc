//
// ********************************************************************
// * License and Disclaimer                                           *
// *                                                                  *
// * The  Geant4 software  is  copyright of the Copyright Holders  of *
// * the Geant4 Collaboration.  It is provided  under  the terms  and *
// * conditions of the Geant4 Software License,  included in the file *
// * LICENSE and available at  http://cern.ch/geant4/license .  These *
// * include a list of copyright holders.                             *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.  Please see the license in the file  LICENSE  and URL above *
// * for the full disclaimer and the limitation of liability.         *
// *                                                                  *
// * This  code  implementation is the result of  the  scientific and *
// * technical work of the GEANT4 collaboration.                      *
// * By using,  copying,  modifying or  distributing the software (or *
// * any work based  on the software)  you  agree  to acknowledge its *
// * use  in  resulting  scientific  publications,  and indicate your *
// * acceptance of all terms of the Geant4 Software license.          *
// ********************************************************************
//
/// \file myRdecay.cc
/// \brief Main program of the radioactivedecay/myRdecay example
//
//
//
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "G4Types.hh"

#include "G4RunManagerFactory.hh"
#include "G4UImanager.hh"
#include "G4SteppingVerbose.hh"
#include "Randomize.hh"

#include "DetectorConstruction.hh"
// #include "PhysicsList.hh"
#include "ActionInitialization.hh"

#include "G4UIExecutive.hh"
#include "G4VisExecutive.hh"
#include "G4UImanager.hh"

#include "G4PhysListFactory.hh"


namespace
{
  void PrintUsage()
  {
    G4cerr << " Usage: " << G4endl;
    G4cerr << " exampleB1c [-m macro ] [-u UIsession] [-t nThreads]" << G4endl;
    G4cerr << "   note: -t option is available only for multi-threaded mode."
           << G4endl;
  }
}


int main(int argc,char** argv)
{    
  std::string pathname = "./test/";
  try{
        system(("mkdir " +  pathname).c_str()); // ???????????????
    }catch(const char* &e){
    }
  
  
  // //????????????????????????????????????????????????HepJamesRandom?????????
  // CLHEP::HepRandom::setTheEngine(new CLHEP::RanecuEngine());
  // //?????????????????????
  // // G4long seed = time(NULL);   //?????????????????????seed
  // CLHEP::HepRandom::setTheSeed(1); //??????????????? 


  // Detect interactive mode (if no arguments) and define UI session
  //

  // Evaluate arguments
  //
  if (argc > 7)
  {
    PrintUsage();
    return 1;
  }



  G4String macro;
  G4String session;
#ifdef G4MULTITHREADED
  G4int nThreads = 8;
#endif

  for (G4int i = 1; i < argc; i = i + 2)
  {
    if (G4String(argv[i]) == "-m")
      macro = argv[i + 1];
    else if (G4String(argv[i]) == "-u")
      session = argv[i + 1];

#ifdef G4MULTITHREADED
    
    else if (G4String(argv[i]) == "-t")
    {
      nThreads = G4UIcommand::ConvertToInt(argv[i + 1]);
    }

#endif
    else
    {
      PrintUsage();
      return 1;
    }
  }

  // Detect interactive mode (if no macro provided) and define UI session
  //
  G4UIExecutive *ui = 0;
  if (!macro.size())
  {
    ui = new G4UIExecutive(argc, argv, session);
  }

  // Optionally: choose a different Random engine...
  // G4Random::setTheEngine(new CLHEP::MTwistEngine);

  // Construct the default run manager
  //
  auto *runManager =
      G4RunManagerFactory::CreateRunManager(G4RunManagerType::Default);
#ifdef G4MULTITHREADED
  if (nThreads > 0)
  {
    runManager->SetNumberOfThreads(nThreads);
  }
#endif

  // Set mandatory initialization classes
  //
  // Detector construction
  runManager->SetUserInitialization(new DetectorConstruction);

  
  // Physics list
  G4PhysListFactory factory;
  G4VModularPhysicsList* physicsList = nullptr;
  // physicsList = factory.GetReferencePhysList("FTFP_BERT_LIV");
  // physicsList = factory.GetReferencePhysList("Shielding");
  physicsList = factory.GetReferencePhysList("FTFP_BERT_HP");
  // physicsList = factory.GetReferencePhysList("QBBC");
  physicsList->SetVerboseLevel(0);
  physicsList->SetDefaultCutValue(0.003*CLHEP::mm); // ????????????????????????
  runManager->SetUserInitialization(physicsList);

  // runManager->SetUserInitialization(new PhysicsList);

  runManager->SetUserInitialization(new ActionInitialization);

  //initialize G4 kernel
  runManager->Initialize();

  //initialize visualization
  G4VisManager* visManager = nullptr;

  //get the pointer to the User Interface manager
  G4UImanager* UImanager = G4UImanager::GetUIpointer();


  if ( macro.size() ) {
    // batch mode
    G4String command = "/control/execute ";
    UImanager->ApplyCommand(command+macro);
  }
  else  {  
    // interactive mode : define UI session
    UImanager->ApplyCommand("/control/execute init_vis.mac");
    if (ui->IsGUI()) {
      UImanager->ApplyCommand("/control/execute gui.mac");
    }
    ui->SessionStart();
    delete ui;
  }


  //job termination
  delete visManager;
  delete runManager;
}

