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
		SteppingAction(const char* detectorEnergyOutFile, const char* detectorEnergyElectronOutFile, DetectorPosition detPos, double detectorAngle);
		virtual ~SteppingAction();

		virtual void UserSteppingAction(const G4Step*);

	private:
		double clearance;

		std::ofstream detEnergyOf;
		std::ofstream detEnergyElectronOf;

		DetectorPosition detPos;
		double detectorAngle;
};

} /* namespace SoEiXRS */

#endif /* SoEiXRS_CLASSES_SteppingAction_HPP_ */
