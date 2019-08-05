#include "DetectorConstruction.hpp"

namespace SoEiXRS {

DetectorConstruction::DetectorConstruction(double targetThickness,
		double targetAngle) :
		G4VUserDetectorConstruction(), targetVolume(0), targetThickness(
				targetThickness), targetAngle(targetAngle), nist(
				G4NistManager::Instance()), vacuum("interGalactic",
				atomicNumber, massOfMole, density, kStateGas, temperature,
				pressure) {


	G4Box* solidWorld = new G4Box("WorldBox", 0.5 * world_sizeXY,
			0.5 * world_sizeXY, 0.5 * world_sizeZ);
	G4LogicalVolume* logicWorld = new G4LogicalVolume(solidWorld, &vacuum,
			"WorldVolume");

	physWorld = new G4PVPlacement(0, G4ThreeVector(), logicWorld,
			"WorldPhysVolume", 0, false, 0, true);

	G4Box* target_box = new G4Box("TargetBox", 2 * cm, 2 * cm,
			targetThickness * mm);
	G4Material* target_mat = nist->FindOrBuildMaterial("G4_W");
	G4LogicalVolume* target_vol = new G4LogicalVolume(target_box, target_mat,
			"TargetVolume");

	G4ThreeVector rotationAxis(0, 1, 0);
	G4RotationMatrix target_rot(rotationAxis,
				targetAngle);
	G4VPhysicalVolume* target_physvol = new G4PVPlacement(&target_rot,
			G4ThreeVector(), target_vol, "TargetPhysVolume", logicWorld, false,
			0, true);

	targetVolume = target_vol;
}

G4VPhysicalVolume* DetectorConstruction::Construct() {
	return physWorld;
}

DetectorConstruction::~DetectorConstruction() {
	// TODO Auto-generated destructor stub
}

} /* namespace SoEiXRS */
