#include "DetectorConstruction.hpp"

namespace SoEiXRS {

DetectorConstruction::DetectorConstruction(double targetThickness,
		double targetAngle, std::string material, std::string absorber,
		double absorberThickness, DetectorPosition detPos) :
		G4VUserDetectorConstruction(), targetThickness(
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

	auto targetWidth = targetThickness * 3 * mm;
	G4Box* target_box = new G4Box("TargetBox", targetWidth, targetWidth,
			targetThickness * mm);
	G4Material* target_mat = nist->FindOrBuildMaterial(material);
	G4LogicalVolume* target_vol = new G4LogicalVolume(target_box, target_mat,
			"TargetVolume");

	CLHEP::HepRotationY target_rot_hep(targetAngle * (3.14159265 / 180));
	G4RotationMatrix* target_rot = new G4RotationMatrix(target_rot_hep);
	G4VPhysicalVolume* target_physvol = new G4PVPlacement(target_rot,
			G4ThreeVector(0, 0, 0), target_vol, "TargetPhysVolume", logicWorld,
			false, 0, true);

	if (absorber != "none") {

		G4Material* absorber_mat = nist->FindOrBuildMaterial(absorber);

		if (detPos == transmission) {

			G4Box* absorber_box = new G4Box("AbsorberBox", targetWidth*1000, targetWidth*1000,
					absorberThickness * mm);
			G4LogicalVolume* absorber_vol = new G4LogicalVolume(absorber_box, absorber_mat,
					"AbsorberVolume");

			G4VPhysicalVolume* absorbersvol = new G4PVPlacement(0,
					G4ThreeVector(0, 0, targetThickness*mm + absorberThickness * mm), absorber_vol,
					"AbsorberPhysVolume", logicWorld, false, 0, true);

			clearanceDistance = 2*(targetThickness*mm + absorberThickness * mm);

		} else if (detPos == perpendicular) {

			auto absorberDistance = 1.1 * cos(targetAngle) * targetWidth + absorberThickness * mm;;
			clearanceDistance = absorberDistance + 2 * absorberThickness * mm;

			G4Box* absorber_box = new G4Box("AbsorberBox",
					absorberThickness * mm, targetWidth * 5, targetWidth * 5);
			G4LogicalVolume* absorber_vol = new G4LogicalVolume(absorber_box,
					absorber_mat, "AbsorberVolume");

			G4VPhysicalVolume* absorbersvol = new G4PVPlacement(0,
					G4ThreeVector(clearanceDistance, 0, 0), absorber_vol, "AbsorberPhysVolume", logicWorld,
					false, 0, true);
		} else {
			std::cout << "wtf" << std::endl;
			exit(-1);
		}

	} else {
		if (detPos == transmission) {
			clearanceDistance = targetThickness * mm;
		} else if (detPos == perpendicular) {
			clearanceDistance = targetWidth;
		}

	}
}

G4VPhysicalVolume* DetectorConstruction::Construct() {
	return physWorld;
}

DetectorConstruction::~DetectorConstruction() {
	// TODO Auto-generated destructor stub
}

} /* namespace SoEiXRS */
