#include "SteppingAction.hpp"

#include "DetectorConstruction.hpp"

#include "G4Step.hh"
#include "G4Event.hh"
#include "G4RunManager.hh"
#include "G4LogicalVolume.hh"

#include "G4Gamma.hh"
#include "G4Electron.hh"

namespace SoEiXRS {

SteppingAction::SteppingAction() :
		G4UserSteppingAction(), targetVolume(0) {
}

SteppingAction::~SteppingAction() {
}

void SteppingAction::UserSteppingAction(const G4Step* step) {

	/*std::cout << step->GetTrack()->GetDefinition() << std::endl;
	std::cout << "e-: " << G4Electron::ElectronDefinition() << std::endl;*/

	if (step->GetTrack()->GetDefinition() != G4Gamma::GammaDefinition()) {
		return; // we are only interested in gamma
	}

	std::cout << "got a gamma!" << std::endl;
	exit(1);

	if (!targetVolume) {
		const SoEiXRS::DetectorConstruction* detectorConstruction =
				static_cast<const SoEiXRS::DetectorConstruction*>(G4RunManager::GetRunManager()->GetUserDetectorConstruction());
		targetVolume = detectorConstruction->GetTargetVolume();
	}

	// get volume of the current step
	G4LogicalVolume* volume =
			step->GetPreStepPoint()->GetTouchableHandle()->GetVolume()->GetLogicalVolume();

	// check if not inside target volume;
	if (volume == targetVolume) {
		return; // as soon as we are outside of the target we will destroy and eventually save the particle based on the direction
	}

	///TODO: get track info and save (if within the opening angle that was specified)

	G4Track *aTrack = step->GetTrack();
	aTrack->SetTrackStatus(fStopAndKill);
}

} /* namespace SoEiXRS */
