#include "DetectorConstruction.hpp"

namespace SoEiXRS {

DetectorConstruction::DetectorConstruction(double targetThickness,
		double targetAngle, std::string material) :
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
	G4Material* target_mat = nist->FindOrBuildMaterial(material);
	G4LogicalVolume* target_vol = new G4LogicalVolume(target_box, target_mat,
			"TargetVolume");

	CLHEP::HepRotationY target_rot_hep(targetAngle*(3.14159265/180));
	G4RotationMatrix* target_rot = new G4RotationMatrix(target_rot_hep);
	G4VPhysicalVolume* target_physvol = new G4PVPlacement(target_rot,
	 G4ThreeVector(0, 0, 0), target_vol, "TargetPhysVolume", logicWorld, false,
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
