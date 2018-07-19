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
//
// $Id: GammaRayTelDetectorMessenger.cc 66508 2012-12-19 10:16:45Z gcosmo $
//
// ------------------------------------------------------------
//      GEANT 4 class implementation file
//      CERN Geneva Switzerland
//
//
//      ------------ GammaRayTelDetectorMessenger ------
//           by F.Longo, R.Giannitrapani & G.Santin (13 nov 2000)
//
// ************************************************************


#include "DetectorConstruction.hh"
#include "DetectorMessenger.hh"

#include "G4UIdirectory.hh"
#include "G4UIcmdWithAString.hh"
#include "G4UIcmdWithAnInteger.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"
#include "G4UIcmdWithoutParameter.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

DetectorMessenger::DetectorMessenger(DetectorConstruction * Det)
  :Detector(Det)

{ 
  detDir = new G4UIdirectory("/payload/");
  detDir->SetGuidance("payload control.");

  // window thickness command

  WindowThickCmd = new G4UIcmdWithADoubleAndUnit("/payload/setWindowThick", this);
  WindowThickCmd->SetGuidance("Set Thickness of the windowow");
  WindowThickCmd->SetParameterName("Size",false);
  WindowThickCmd->SetRange("Size>=0.");
  WindowThickCmd->SetUnitCategory("Length");
  WindowThickCmd->AvailableForStates(G4State_Idle);

  WindowMaterCmd = new G4UIcmdWithAString("/payload/setWindowMat",this);
  WindowMaterCmd->SetGuidance("Select Material of the Converter.");
  WindowMaterCmd->SetParameterName("choice",false);
  WindowMaterCmd->AvailableForStates(G4State_Idle);

  // update Payload
  UpdateCmd = new G4UIcmdWithoutParameter("/payload/update",this);
  UpdateCmd->SetGuidance("Update payload geometry.");
  UpdateCmd->SetGuidance("This command MUST be applied before \"beamOn\" ");
  UpdateCmd->SetGuidance("if you changed geometrical value(s).");
  UpdateCmd->AvailableForStates(G4State_Idle);


  
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

DetectorMessenger::~DetectorMessenger()
{
  delete WindowThickCmd;
  delete detDir;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

void DetectorMessenger::SetNewValue(G4UIcommand* command,G4String newValue)
{ 
  if (command == WindowThickCmd)
    {Detector->SetWindowThickness(WindowThickCmd->GetNewDoubleValue(newValue));}

  if( command == WindowMaterCmd )
    { Detector->SetWindowMaterial(newValue);}
   

    if( command == UpdateCmd )
    { Detector->UpdateGeometry(); }

  
}









