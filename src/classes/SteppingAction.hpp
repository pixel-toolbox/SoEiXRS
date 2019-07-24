#ifndef SoEiXRS_CLASSES_SteppingAction_HPP_
#define SoEiXRS_CLASSES_SteppingAction_HPP_ 1

#include "G4UserSteppingAction.hh"
#include "globals.hh"

class B1EventAction;

class G4LogicalVolume;

namespace SoEiXRS {

class SteppingAction : public G4UserSteppingAction {

	public:
		SteppingAction();
		virtual ~SteppingAction();

		virtual void UserSteppingAction(const G4Step*);

	private:
		G4LogicalVolume* targetVolume;
};

} /* namespace SoEiXRS */

#endif /* SoEiXRS_CLASSES_SteppingAction_HPP_ */
