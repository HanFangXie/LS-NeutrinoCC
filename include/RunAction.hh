#ifndef RunAction_h
#define RunAction_h

#include "G4UserRunAction.hh"
#include "G4Run.hh"
#include <fstream>

namespace B1 {

class RunAction : public G4UserRunAction
{
public:
    RunAction();
    virtual ~RunAction();

    virtual void BeginOfRunAction(const G4Run* run);
    virtual void EndOfRunAction(const G4Run* run);

    void AddGammaEdep(G4double edep);
    void AddPositronEdep(G4double edep);
    void AddElectronEdep(G4double edep);


private:
    std::ofstream outFile;

    G4double totalGammaEdep;
    G4double totalPositronEdep;
    G4double totalElectronEdep;

};

}

#endif
