/// \file B1/src/DetectorConstruction.cc
/// \brief Implementation of the B1::DetectorConstruction class

#include "DetectorConstruction.hh"
#include "G4Material.hh"
#include "G4NistManager.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4RunManager.hh"
#include "G4SystemOfUnits.hh"
#include "G4PhysicalConstants.hh"
#include "G4Tubs.hh"
#include "G4Box.hh"
#include "G4VisAttributes.hh"

namespace B1
{

G4LogicalVolume* logicWorld = nullptr;
G4LogicalVolume* logicAcrylicShell = nullptr;
G4LogicalVolume* logicGdLS = nullptr;
G4LogicalVolume* logicAcrylicEndcap = nullptr;

G4VPhysicalVolume* DetectorConstruction::Construct() {
    G4NistManager* nist = G4NistManager::Instance();

    // Define materials
    G4Material* Acrylic = nist->FindOrBuildMaterial("G4_PLEXIGLASS");
    G4Material* LAB = new G4Material("LAB", 0.86 * g/cm3, 2);
    LAB->AddElement(nist->FindOrBuildElement("C"), 18);
    LAB->AddElement(nist->FindOrBuildElement("H"), 30);

    G4Material* GdCl3 = new G4Material("GdCl3", 4.5 * g/cm3, 2);
    GdCl3->AddElement(nist->FindOrBuildElement("Gd"), 1);
    GdCl3->AddElement(nist->FindOrBuildElement("Cl"), 3);

    G4Material* GdLS = new G4Material("GdLS", 0.87 * g/cm3, 2);
    GdLS->AddMaterial(LAB, 99.8 * perCent);
    GdLS->AddMaterial(GdCl3, 0.2 * perCent);

    // Define world volume
    G4double world_sizeXY = 3 * m;
    G4double world_sizeZ = 3 * m;
    G4Material* world_mat = nist->FindOrBuildMaterial("G4_Galactic");

    G4Box* solidWorld = new G4Box("World", world_sizeXY / 2, world_sizeXY / 2, world_sizeZ / 2);
    logicWorld = new G4LogicalVolume(solidWorld, world_mat, "World");
    G4VPhysicalVolume* physWorld = new G4PVPlacement(nullptr, G4ThreeVector(), logicWorld, "World", nullptr, false, 0);

    // Define acrylic shell
    G4double innerRadius = 1.0 * m;
    G4double outerRadius = 1.02 * m;
    G4double halfHeight = 0.5 * m;

    G4Tubs* solidAcrylicShell = new G4Tubs("AcrylicShell", innerRadius, outerRadius, halfHeight, 0., 360.*deg);
    logicAcrylicShell = new G4LogicalVolume(solidAcrylicShell, Acrylic, "AcrylicShell");

    // Define GdLS
    G4Tubs* solidGdLS = new G4Tubs("GdLS", 0.0*m, innerRadius - 0.01*m, halfHeight, 0., 360.*deg);
    logicGdLS = new G4LogicalVolume(solidGdLS, GdLS, "GdLS");

    // Add rotation matrix to lie the cylinder down (axis along X)
    G4RotationMatrix* rotCyl = new G4RotationMatrix();
    rotCyl->rotateY(90. * deg);

    // Place acrylic shell and GdLS
    new G4PVPlacement(rotCyl, G4ThreeVector(), logicAcrylicShell, "AcrylicShell", logicWorld, false, 0);
    new G4PVPlacement(rotCyl, G4ThreeVector(), logicGdLS, "GdLS", logicWorld, false, 0);

    // Define Acrylic Endcaps (left and right caps for the horizontal cylinder)
    G4double endcapRadius = outerRadius;
    G4double endcapThickness = 0.01 * m;

    G4Tubs* solidAcrylicEndcap = new G4Tubs("AcrylicEndcap", 0.0, endcapRadius, endcapThickness / 2, 0., 360. * deg);
    logicAcrylicEndcap = new G4LogicalVolume(solidAcrylicEndcap, Acrylic, "AcrylicEndcap");

    // Place the two endcaps at both ends along X-axis
    new G4PVPlacement(rotCyl,
                      G4ThreeVector(-halfHeight - endcapThickness / 2, 0, 0),  // left cap
                      logicAcrylicEndcap, "AcrylicEndcapLeft", logicWorld, false, 0);

    new G4PVPlacement(rotCyl,
                      G4ThreeVector(+halfHeight + endcapThickness / 2, 0, 0),  // right cap
                      logicAcrylicEndcap, "AcrylicEndcapRight", logicWorld, false, 1);


    fScoringVolume = logicGdLS;
    return physWorld;
}

}
