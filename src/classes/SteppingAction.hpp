#ifndef SoEiXRS_CLASSES_SteppingAction_HPP_
#define SoEiXRS_CLASSES_SteppingAction_HPP_ 1

#include "G4UserSteppingAction.hh"
#include "globals.hh"
#include "DetectorPosition.hh"

#include <fstream>

class B1EventAction;

class G4LogicalVolume;

namespace SoEiXRS {

class SteppingAction : public G4UserSteppingAction {

	public:
		SteppingAction(const char* allEnergyOutFile, const char* detectorEnergyOutFile, DetectorPosition detPos, double detectorAngle);
		virtual ~SteppingAction();

		virtual void UserSteppingAction(const G4Step*);

	private:
		G4LogicalVolume* targetVolume;

		std::ofstream allEnergyOf;
		std::ofstream detEnergyOf;

		DetectorPosition detPos;
		double detectorAngle;
};

} /* namespace SoEiXRS */

#endif /* SoEiXRS_CLASSES_SteppingAction_HPP_ */
