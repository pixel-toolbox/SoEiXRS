#ifndef SoEiXRS_CLASSES_DETECTORCONSTRUCTION_HPP_
#define SoEiXRS_CLASSES_DETECTORCONSTRUCTION_HPP_ 1

#include "G4VUserDetectorConstruction.hh"

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

class DetectorConstruction : public G4VUserDetectorConstruction {

	public:
		DetectorConstruction(double targetThickness, double targetAngle);

		G4VPhysicalVolume* Construct();

	    G4LogicalVolume* GetTargetVolume() const { return targetVolume; }

		virtual ~DetectorConstruction();

	protected:
	    G4LogicalVolume*  targetVolume;

		double targetThickness;
		double targetAngle;

	private:
		// Constant Stuff

		const G4double world_sizeXY = 21 * cm;
		const G4double world_sizeZ = 21 * cm;

		const G4double atomicNumber = 1.;
		const G4double massOfMole = 1.008 * g / mole;
		const G4double density = 1.e-25 * g / cm3;
		const G4double temperature = 2.73 * kelvin;
		const G4double pressure = 3.e-18 * pascal;

		// Stuff moved here from DetectorConstruction to eliminate memory leaks

		G4NistManager* nist;
		G4Material vacuum;

		G4VPhysicalVolume* physWorld;

};

} /* namespace SoEiXRS */

#endif /* SoEiXRS_CLASSES_DETECTORCONSTRUCTION_HPP_ */
