#ifndef STEPPINGACTION_HH
#define STEPPINGACTION_HH

#include "G4UserSteppingAction.hh"
#include "G4Track.hh"
#include "G4Step.hh"
#include <fstream>

namespace B1 {

class EventAction;

class SteppingAction : public G4UserSteppingAction {
public:
    SteppingAction(EventAction* eventAction);
    virtual ~SteppingAction();

    virtual void UserSteppingAction(const G4Step* step) override;

private:
    EventAction* fEventAction;
    std::ofstream outMuonNuclear;
};

} // namespace B1
#endif
