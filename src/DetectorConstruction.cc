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
// $Id: DetectorConstruction.cc 94307 2015-11-11 13:42:46Z gcosmo $
//
/// \file DetectorConstruction.cc
/// \brief Implementation of the DetectorConstruction class

#include "DetectorConstruction.hh"
#include "DetectorMessenger.hh"

#include "G4RunManager.hh"
#include "G4NistManager.hh"
#include "G4Box.hh"
#include "G4Cons.hh"
#include "G4Orb.hh"
#include "G4Sphere.hh"
#include "G4Trd.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4SystemOfUnits.hh"



//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

DetectorConstruction::DetectorConstruction()
: G4VUserDetectorConstruction(),
  window_volume(0),
  fScoringVolume(0)
{ 
  // set some default parameters
  WindowThickness = 0.5/2.*mm;
  G4NistManager* nist = G4NistManager::Instance();
  window_material = nist->FindOrBuildMaterial("G4_W");

  detectorMessenger = new DetectorMessenger(this);

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

DetectorConstruction::~DetectorConstruction()
{ delete detectorMessenger; }

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4VPhysicalVolume* DetectorConstruction::Construct()
{  
  // Get nist material manager
  // G4NistManager* nist = G4NistManager::Instance();
  
  // Envelope parameters
  //
  G4double env_sizeXY = 20*cm, env_sizeZ = 30*cm;
  // G4Material* env_mat = nist->FindOrBuildMaterial("G4_");

    // Material: Vacuum
  G4Material* env_mat = new G4Material("Vacuum",
              1.0 , 1.01*g/mole, 1.0E-25*g/cm3,
              kStateGas, 2.73*kelvin, 3.0E-18*pascal );
   
  // Option to switch on/off checking of volumes overlaps
  //
  G4bool checkOverlaps = true;

  //     
  // World
  //
  G4double world_sizeXY = 1.2*env_sizeXY;
  G4double world_sizeZ  = 1.2*env_sizeZ;
  // G4Material* world_mat = nist->FindOrBuildMaterial("G4_AIR");
  G4Material* world_mat = new G4Material("Vacuum",
              1.0 , 1.01*g/mole, 1.0E-25*g/cm3,
              kStateGas, 2.73*kelvin, 3.0E-18*pascal );
  
  G4Box* solidWorld =    
    new G4Box("World",                       //its name
       0.5*world_sizeXY, 0.5*world_sizeXY, 0.5*world_sizeZ);     //its size
      
  G4LogicalVolume* logicWorld =                         
    new G4LogicalVolume(solidWorld,          //its solid
                        world_mat,           //its material
                        "World");            //its name
                                   
  G4VPhysicalVolume* physWorld = 
    new G4PVPlacement(0,                     //no rotation
                      G4ThreeVector(),       //at (0,0,0)
                      logicWorld,            //its logical volume
                      "World",               //its name
                      0,                     //its mother  volume
                      false,                 //no boolean operation
                      0,                     //copy number
                      checkOverlaps);        //overlaps checking
                     
  //     
  // Envelope
  //  
  G4Box* solidEnv =    
    new G4Box("Envelope",                    //its name
        0.5*env_sizeXY, 0.5*env_sizeXY, 0.5*env_sizeZ); //its size
      
  G4LogicalVolume* logicEnv =                         
    new G4LogicalVolume(solidEnv,            //its solid
                        env_mat,             //its material
                        "Envelope");         //its name
               
  new G4PVPlacement(0,                       //no rotation
                    G4ThreeVector(),         //at (0,0,0)
                    logicEnv,                //its logical volume
                    "Envelope",              //its name
                    logicWorld,              //its mother  volume
                    false,                   //no boolean operation
                    0,                       //copy number
                    checkOverlaps);          //overlaps checking
 

   // Conical section shape       
  G4double detector_radius = 4.*cm;
  G4double detector_depth =  2.*cm;
      
  // G4double window_depth = WindowThickness; // 0.5/2.*mm;

  // Window

  // window_material = nist->FindOrBuildMaterial("G4_W");
  G4ThreeVector window_pos = G4ThreeVector(0, 0, 3*cm);
        

  G4Cons* window_shape = new G4Cons("window", 
    0, detector_radius, 0, detector_radius, WindowThickness/2.,
    0*deg, 360*deg);
                      
  window_volume =                         
    new G4LogicalVolume(window_shape,        //its solid
                        window_material,     //its material
                        "window");           //its name
               
  new G4PVPlacement(0,                       //no rotation
                    window_pos,              //at position
                    window_volume,           //its logical volume
                    "window",                //its name
                    logicEnv,                //its mother  volume
                    false,                   //no boolean operation
                    0,                       //copy number
                    checkOverlaps);          //overlaps checking

      
  // Detector

  // Silicon detector
  // G4Material* detector_mat = nist->FindOrBuildMaterial("G4_Si");

  // CZT detector
  G4Element* Cd = new G4Element("Cadmium","Cd",48., 112.41*g/mole);
  G4Element* Zn = new G4Element("Zinc","Zn", 30., 65.38*g/mole);
  G4Element* Te = new G4Element("Tellurium","Te", 52., 127.60*g/mole);
  G4Material* CZT = new G4Material("CZT", 5.8*g/cm3, 3);
  CZT->AddElement(Cd, 48*perCent);
  CZT->AddElement(Zn, 02*perCent);
  CZT->AddElement(Te, 50*perCent);


  G4ThreeVector detector_pos = G4ThreeVector(0, 0, 0);


  G4Cons* detector_shape = new G4Cons("detector", 
    0, detector_radius, 0, detector_radius, detector_depth,
    0*deg, 360*deg);

  G4LogicalVolume* detector_vol =                         
    new G4LogicalVolume(detector_shape,      //its solid
                        CZT,                 //its material
                        "detector");         //its name
               
  new G4PVPlacement(0,                       //no rotation
                    detector_pos,            //at position
                    detector_vol,            //its logical volume
                    "detector",              //its name
                    logicEnv,                //its mother  volume
                    false,                   //no boolean operation
                    0,                       //copy number
                    checkOverlaps);          //overlaps checking
                
  // Set Shape2 as scoring volume 
  fScoringVolume = detector_vol;

  //always return the physical World
  return physWorld;
}



void DetectorConstruction::SetWindowThickness(G4double val)
{
  WindowThickness = val;
}  

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

void DetectorConstruction::SetWindowMaterial(G4String materialChoice)
{
  // search the material by its name   
  // G4Material* pttoMaterial = G4Material::GetMaterial(materialChoice);     
  G4NistManager* nist = G4NistManager::Instance();

  if (materialChoice=="Al") {
    G4Material* pttoMaterial = nist->FindOrBuildMaterial("G4_Al");

    window_material = pttoMaterial;
    window_volume->SetMaterial(pttoMaterial);
    G4cout << "using Aluminum window" << G4endl;
  }
   

  if (materialChoice=="W") {
    G4Material* pttoMaterial = nist->FindOrBuildMaterial("G4_W");

    window_material = pttoMaterial;
    window_volume->SetMaterial(pttoMaterial);
    G4cout << "using Tungsten window" << G4endl;
  }       
}


void DetectorConstruction::UpdateGeometry()
{
  //  delete payloadSD;
  G4RunManager::GetRunManager()->DefineWorldVolume(Construct());
  // G4RunManager::GetRunManager()->PhysicsHasBeenModified();

  G4RunManager::GetRunManager()->GeometryHasBeenModified();
  G4RunManager::GetRunManager()->ReinitializeGeometry();
}



//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
