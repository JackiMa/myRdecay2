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
/// \file DetectorConstruction.cc
/// \brief Implementation of the DetectorConstruction class
//
//
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "DetectorConstruction.hh"

#include "G4NistManager.hh"
#include "G4Box.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4SystemOfUnits.hh"
#include "G4VisAttributes.hh"

// 哈希运算
typedef std::uint64_t hash_t;
constexpr hash_t prime = 0x100000001B3ull; // 哈希运算用到的常数
constexpr hash_t basis = 0xCBF29CE484222325ull;

// 定义哈希运算

hash_t hash_(char const *str)
{
  hash_t ret{basis};

  while (*str)
  {
    ret ^= *str;
    ret *= prime;
    str++;
  }

  return ret;
}
constexpr hash_t hash_compile_time(char const *str, hash_t last_value = basis)
{
  return *str ? hash_compile_time(str + 1, (*str ^ last_value) * prime) : last_value;
}
constexpr unsigned long long operator"" _hash(char const *p, size_t)
{
  return hash_compile_time(p);
}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

DetectorConstruction::DetectorConstruction()
    : G4VUserDetectorConstruction()
{
  fWorldSize = 12 * cm;
  fCrystalWidth = 2*cm;
  fCrystalLength = 6*cm;
  fCrystalHeight = 4*cm;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

DetectorConstruction::~DetectorConstruction()
{
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4VPhysicalVolume *DetectorConstruction::Construct()
{
  G4bool checkOverlaps = true;

  // World
  G4Material *Vacuum = GetMaterial("Vacuum");
  G4Box *solidWorld = new G4Box("World", fWorldSize / 2, fWorldSize / 2, fWorldSize / 2);
  G4LogicalVolume *logicWorld = new G4LogicalVolume(solidWorld, Vacuum, "World");
  G4VPhysicalVolume *physiWorld = new G4PVPlacement(0, G4ThreeVector(), logicWorld, "World", 0, false, checkOverlaps);

  // Crystal
  G4String CrystalName = "GAGG";
  G4Material *CrystalMaterial = GetMaterial(CrystalName);
  G4Box *solidCrystal = new G4Box((CrystalName+"Solid").c_str(), fCrystalLength / 2, fCrystalHeight / 2, fCrystalWidth / 2);
  G4LogicalVolume *logicCrystal = new G4LogicalVolume(solidCrystal, CrystalMaterial, (CrystalName+"Logic").c_str());

  new G4PVPlacement(0, G4ThreeVector(0,0,-5*cm), logicCrystal, (CrystalName+"Physic").c_str(), logicWorld, false, checkOverlaps);

// set colour to logicVolume
  G4VisAttributes* transgrey = new G4VisAttributes(G4Colour(0.64,0.7,0.5));
  transgrey->SetForceSolid(true);
  logicCrystal->SetVisAttributes(transgrey);

  // always return the physical World
  return physiWorld;
}

G4Material *DetectorConstruction::GetMaterial(G4String MaterialName) // find element
{
  G4int natoms, ncomponents;
  G4Material *Mat;
  G4String mat_name;
  G4NistManager *nist = G4NistManager::Instance();

  switch (hash_(MaterialName))
  { // 通过计算数组的hash值，来实现switch string
  case "GAGG"_hash:
    if ("GAGG" == MaterialName) // 再做判断，避免不同的数组偶然hash到了相同的值
    {
      G4Element *eleGd = nist->FindOrBuildElement("Gd");
      G4Element *eleAl = nist->FindOrBuildElement("Al");
      G4Element *eleO = nist->FindOrBuildElement("O");
      G4Element *eleGa = nist->FindOrBuildElement("Ga");
      G4double density_GAGG = 6.6 * g / cm3;
      Mat = new G4Material(mat_name = "GAGG", density_GAGG, ncomponents = 4);
      Mat->AddElement(eleGd, natoms = 3);
      Mat->AddElement(eleAl, natoms = 2);
      Mat->AddElement(eleGa, natoms = 3);
      Mat->AddElement(eleO, natoms = 12);
      break;
    }

  case "BGO"_hash:
    if ("BGO" == MaterialName)
    {
      G4Element *eleBi = nist->FindOrBuildElement("Bi");
      G4Element *eleGe = nist->FindOrBuildElement("Ge");
      G4Element *eleO = nist->FindOrBuildElement("O");
      G4double density_BGO = 7.13 * g / cm3;
      Mat = new G4Material(mat_name = "BGO", density_BGO, ncomponents = 3);
      Mat->AddElement(eleBi, natoms = 4);
      Mat->AddElement(eleGe, natoms = 3);
      Mat->AddElement(eleO, natoms = 12);
      break;
    }

  case "LYSO"_hash:
    if ("LYSO" == MaterialName)
    {
      G4Element *eleLu = nist->FindOrBuildElement("Lu");
      G4Element *eleSi = nist->FindOrBuildElement("Si");
      G4Element *eleO = nist->FindOrBuildElement("O");
      G4double density_LYSO = 7.3 * g / cm3;
      Mat = new G4Material(mat_name = "LYSO", density_LYSO, ncomponents = 3);
      Mat->AddElement(eleLu, natoms = 2);
      Mat->AddElement(eleSi, natoms = 1);
      Mat->AddElement(eleO, natoms = 5);
      break;
    }

  case "CsI"_hash:
    if ("CsI" == MaterialName)
    {
      G4Element *eleCs = nist->FindOrBuildElement("Cs");
      G4Element *eleI = nist->FindOrBuildElement("I");
      G4double density_CsI = 4.51 * g / cm3;
      Mat = new G4Material(mat_name = "CsI", density_CsI, ncomponents = 2);
      Mat->AddElement(eleCs, natoms = 1);
      Mat->AddElement(eleI, natoms = 1);
      break;
    }

  case "Vacuum"_hash:
    if ("Vacuum" == MaterialName)
    {
      Mat = nist->FindOrBuildMaterial("G4_Galactic");
      break;
    }

  default:
    throw "\n --- Error! couldn't find the material defined ---\n";
  }

  return Mat;
  //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
}
