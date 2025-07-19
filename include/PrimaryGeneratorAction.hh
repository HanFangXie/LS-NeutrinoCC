#ifndef PrimaryGeneratorAction_h
#define PrimaryGeneratorAction_h
#include "G4VUserPrimaryGeneratorAction.hh"

#include "G4ParticleGun.hh"
#include "G4Gamma.hh"
#include "G4ParticleDefinition.hh"
#include "G4ThreeVector.hh"
#include "G4Event.hh"

namespace B1 {
class PrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction
{
public:
    PrimaryGeneratorAction();
    virtual ~PrimaryGeneratorAction();

    virtual void GeneratePrimaries(G4Event* event) override;

private:
    G4ParticleGun* fParticleGun;

    G4ParticleDefinition* electron;
    double SampleElectronEnergy();
    G4ThreeVector SampleElectronDirection();

    void GenerateGammaPrimaries(G4Event* event, const G4ThreeVector& pos);  // 添加pos参数
    G4ParticleDefinition* gamma; // add defition of gamma


};
}

#endif
