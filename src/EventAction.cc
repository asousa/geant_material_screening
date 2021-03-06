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
// $Id: EventAction.cc 93886 2015-11-03 08:28:26Z gcosmo $
//
/// \file EventAction.cc
/// \brief Implementation of the EventAction class

#include "EventAction.hh"
#include "RunAction.hh"
    
#include "G4Event.hh"
#include "G4RunManager.hh"
#include "G4SystemOfUnits.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

EventAction::EventAction(RunAction* runAction)
: G4UserEventAction(),
  fRunAction(runAction),
  fEdep(0.),
  fEdep_window(0.),
  fEdep_entering_detector(0.)
{} 

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

EventAction::~EventAction()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void EventAction::AddEdep(G4double edep)
{
    edep += fEdep;
}

// Cumulative sum of energy deposited across a single event
void EventAction::AddEdep_multiple(G4String solid, G4double edep)
{
    if (solid =="detector") { fEdep += edep;}
    if (solid =="window") { fEdep_window += edep;}
    if (solid =="entering") {
      // G4cout << "Logging " << edep << "eV" << G4endl;
      fEdep_entering_detector += edep;}


}



void EventAction::BeginOfEventAction(const G4Event*)
{    
  fEdep = 0.;
  fEdep_window = 0.;
  fEdep_entering_detector = 0.;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void EventAction::EndOfEventAction(const G4Event* event)
{  

  // accumulate statistics in run action
  fRunAction->AddEdep(fEdep);

  G4AnalysisManager* man = G4AnalysisManager::Instance();

  // Fill histograms
  // Source:
  G4double init_energy = event->GetPrimaryVertex()->GetPrimary()->GetKineticEnergy();
  man->FillH1(1,init_energy);

  // Detector:
  man->FillH1(2,fEdep);

  // Window:
  man->FillH1(3,fEdep_window);
  man->FillH1(4,fEdep_entering_detector);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
