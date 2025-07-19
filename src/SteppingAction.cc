#include "SteppingAction.hh"
#include "EventAction.hh"
#include "G4Step.hh"
#include "G4Track.hh"
#include "G4Gamma.hh"
#include "G4Electron.hh"
#include "G4SystemOfUnits.hh"
#include "Randomize.hh"
#include "G4VProcess.hh"
#include <iostream>

namespace B1 {

SteppingAction::SteppingAction(EventAction* eventAction)
    : fEventAction(eventAction){}

// 必须实现虚析构函数
SteppingAction::~SteppingAction() {
    if(outMuonNuclear.is_open()) {
        outMuonNuclear.close();
    }
}

void SteppingAction::UserSteppingAction(const G4Step* step)
{
    G4Track* track = step->GetTrack();
    auto particle = track->GetDefinition();

    // 电子处理
    if (particle == G4Electron::Definition()) {
        fEventAction->AddElectronEdep(step->GetTotalEnergyDeposit());


        G4ThreeVector pos = step->GetPostStepPoint()->GetPosition();
        fEventAction->AddElectronHit(pos.x(), pos.y(), pos.z());

        if (!fEventAction->IsElectronPosSet()) {
            fEventAction->SetElectronPos(pos.x(), pos.y(), pos.z());
        }
    }

    // gamma处理（模拟探测效率）
    if (particle == G4Gamma::Definition()) {
//      G4cout << "Pre-filter gamma: " << track->GetKineticEnergy()/MeV << " MeV" << G4endl;
        if (G4UniformRand() < 0.8) {
//          G4cout << "Detected gamma: " << track->GetKineticEnergy()/MeV << " MeV" << G4endl;
            fEventAction->AddGammaEnergy(track->GetKineticEnergy());
        } else {
            track->SetTrackStatus(fStopAndKill);
        }
    }

}

} // namespace B1
