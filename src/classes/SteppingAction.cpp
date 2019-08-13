#include "SteppingAction.hpp"

#include "DetectorConstruction.hpp"
#include "DetectorPosition.hh"

#include "G4Step.hh"
#include "G4Event.hh"
#include "G4RunManager.hh"
#include "G4LogicalVolume.hh"

#include "G4Gamma.hh"
#include "G4Electron.hh"

#define USE_MAX true

namespace SoEiXRS {

SteppingAction::SteppingAction(const char* allEnergyOutFile,
		const char* detectorEnergyOutFile, DetectorPosition detPos, double detectorAngle) :
		G4UserSteppingAction(), targetVolume(0), allEnergyOf(allEnergyOutFile), detEnergyOf(
				detectorEnergyOutFile), detPos(detPos), detectorAngle(detectorAngle) {
}

SteppingAction::~SteppingAction() {
	allEnergyOf.close();
	detEnergyOf.close();
}

void SteppingAction::UserSteppingAction(const G4Step* step) {

	if (step->GetTrack()->GetDefinition() != G4Gamma::GammaDefinition()) {
		return;
	}

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

	auto energy = step->GetTrack()->GetKineticEnergy() / keV;

	// save 4pi Energy:
	allEnergyOf << energy << std::endl;

	G4ThreeVector direction = step->GetTrack()->GetMomentumDirection();

	auto x = direction.getX();
	auto y = direction.getY();
	auto z = direction.getZ();

	// save angle dependent Energy if necessary:
	if (detPos == transmission) {
		if (z>0 && detectorAngle >= (2*atan(
			#if USE_MAX
				std::max(x, y)
			#else
				sqrt(x*x+y*y)
			#endif
				/z)*(180/3.14159265)) ) {
			detEnergyOf << energy << " " << x << " " << y << " " << z << std::endl;
		}
	} else if (detPos == perpendicular) {
		if (x>0 && detectorAngle >= (2*atan(
				sqrt(z*z+y*y)/x)
		        *(180/3.14159265)) ) {
			detEnergyOf << energy << " " << x << " " << y << " " << z << std::endl;
		}
	} else {
		//WTF
		throw "wtf";
	}

	G4Track *aTrack = step->GetTrack();
	aTrack->SetTrackStatus(fStopAndKill);
}

} /* namespace SoEiXRS */
