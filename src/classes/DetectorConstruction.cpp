#include "DetectorConstruction.hpp"

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

namespace SoEiXRS {

DetectorConstruction::DetectorConstruction(double targetThickness,
		double targetAngle) :
		G4VUserDetectorConstruction(), targetVolume(0), targetThickness(
				targetThickness), targetAngle(targetAngle) {
}

G4VPhysicalVolume* DetectorConstruction::Construct() {
	G4NistManager* nist = G4NistManager::Instance();

	G4double world_sizeXY = 21 * cm;
	G4double world_sizeZ = 21 * cm;

	G4double atomicNumber = 1.;
	G4double massOfMole = 1.008 * g / mole;
	G4double density = 1.e-25 * g / cm3;
	G4double temperature = 2.73 * kelvin;
	G4double pressure = 3.e-18 * pascal;
	G4Material* vacuum = new G4Material("interGalactic", atomicNumber, massOfMole, density, kStateGas, temperature, pressure);

	G4Box* solidWorld = new G4Box("WorldBox", 0.5 * world_sizeXY,
			0.5 * world_sizeXY, 0.5 * world_sizeZ);
	G4LogicalVolume* logicWorld = new G4LogicalVolume(solidWorld, vacuum,
			"WorldVolume");
	G4VPhysicalVolume* physWorld = new G4PVPlacement(0, G4ThreeVector(),
			logicWorld, "WorldPhysVolume", 0, false, 0, true);

	G4Box* target_box = new G4Box("TargetBox", 2 * cm, 2 * cm,
			targetThickness * mm);
	G4Material* target_mat = nist->FindOrBuildMaterial("G4_W");
	G4LogicalVolume* target_vol = new G4LogicalVolume(target_box, target_mat,
			"TargetVolume");
	G4ThreeVector* rotationAxis = new G4ThreeVector(0, 1, 0);
	G4RotationMatrix* target_rot = new G4RotationMatrix(*rotationAxis, targetAngle);
	G4VPhysicalVolume* target_physvol = new G4PVPlacement(target_rot, G4ThreeVector(),
			target_vol, "TargetPhysVolume", logicWorld, false, 0, true);

	targetVolume = target_vol;

	return physWorld;
}

DetectorConstruction::~DetectorConstruction() {
	// TODO Auto-generated destructor stub
}

} /* namespace SoEiXRS */
