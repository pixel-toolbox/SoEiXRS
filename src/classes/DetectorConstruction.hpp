#ifndef SoEiXRS_CLASSES_DETECTORCONSTRUCTION_HPP_
#define SoEiXRS_CLASSES_DETECTORCONSTRUCTION_HPP_ 1

#include "G4VUserDetectorConstruction.hh"

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
};

} /* namespace SoEiXRS */

#endif /* SoEiXRS_CLASSES_DETECTORCONSTRUCTION_HPP_ */
