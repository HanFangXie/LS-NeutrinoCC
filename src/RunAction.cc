/// \file B1/src/RunAction.cc
/// \brief Implementation of the B1::RunAction class

#include "RunAction.hh"
#include "PrimaryGeneratorAction.hh"
#include "DetectorConstruction.hh"

#include "EventAction.hh"
#include "G4Run.hh"
#include "G4RunManager.hh"
#include "G4AccumulableManager.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4LogicalVolume.hh"
#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"
#include <iostream>
#include <fstream>

#include "G4MuonMinus.hh"
#include "G4ProcessManager.hh"

namespace B1 {

RunAction::RunAction()
{

}

RunAction::~RunAction()
{
    if (outFile.is_open()) {
        outFile.close();
    }
}

void RunAction::BeginOfRunAction(const G4Run* run)
{
  auto mu = G4MuonMinus::MuonMinusDefinition();
  auto pmu = mu->GetProcessManager();
  auto plist = pmu->GetProcessList();

  for (std::size_t i = 0; i < plist->size(); ++i) {
      auto proc = (*plist)[i];
  }
}

void RunAction::EndOfRunAction(const G4Run* run)
{

  totalGammaEdep = 0.;
  totalPositronEdep = 0.;
  totalElectronEdep = 0.;
}

void RunAction::AddGammaEdep(G4double edep)
{
    totalGammaEdep += edep;
}

void RunAction::AddPositronEdep(G4double edep)
{
    totalPositronEdep += edep;
}

void RunAction::AddElectronEdep(G4double edep)
{
    totalElectronEdep += edep;
}

} // namespace B1
