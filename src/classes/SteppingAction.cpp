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

SteppingAction::SteppingAction(const char* detectorEnergyOutFile,
		const char* detectorEnergyElectronOutFile, DetectorPosition detPos,
		double detectorAngle) :
		G4UserSteppingAction(), clearance(-1), detEnergyOf(
				detectorEnergyOutFile), detEnergyElectronOf(
				detectorEnergyElectronOutFile), detPos(detPos), detectorAngle(
				detectorAngle) {
}

SteppingAction::~SteppingAction() {
	detEnergyOf.close();
	detEnergyElectronOf.close();
}

void SteppingAction::UserSteppingAction(const G4Step* step) {

	if (clearance == -1) {
		const SoEiXRS::DetectorConstruction* detectorConstruction =
				static_cast<const SoEiXRS::DetectorConstruction*>(G4RunManager::GetRunManager()->GetUserDetectorConstruction());
		clearance = detectorConstruction->getClearanceDistance();
	}

	G4ThreeVector direction = step->GetTrack()->GetMomentumDirection();

	auto x = direction.getX();
	auto y = direction.getY();
	auto z = direction.getZ();

	// We only want to remember stuff behind the absorber (or the detector if no absorber present
	if (detPos == transmission) {
		if (step->GetTrack()->GetPosition().z() < clearance) {
			return;
		}
	} else if (detPos == perpendicular) {
		if (step->GetTrack()->GetPosition().x() < clearance) {
			return;
		}
	}

	auto energy = step->GetTrack()->GetKineticEnergy() / keV;

	// save angle dependent Energy if necessary:
	if (detPos == transmission) {
		if (z > 0 && detectorAngle >= (2 * atan(
#if USE_MAX
				std::max(x, y)
#else
				sqrt(x*x+y*y)
#endif
                /z)*(180/3.14159265)) ) {
			if (step->GetTrack()->GetDefinition() == G4Gamma::GammaDefinition()) {
				detEnergyOf << energy << " " << x << " " << y << " " << z << std::endl;
			} else if (step->GetTrack()->GetDefinition() == G4Electron::ElectronDefinition()) {
				detEnergyElectronOf << energy << " " << x << " " << y << " " << z << std::endl;
			}
		}
	} else if (detPos == perpendicular) {
		if (x>0 && detectorAngle >= (2*atan(
#if USE_MAX
		        std::max(x, y)
#else
		        sqrt(x*x+y*y)
#endif
		        /x)*(180/3.14159265)) ) {
			if (step->GetTrack()->GetDefinition() == G4Gamma::GammaDefinition()) {
				detEnergyOf << energy << " " << x << " " << y << " " << z << std::endl;
			} else if (step->GetTrack()->GetDefinition() == G4Electron::ElectronDefinition()) {
				detEnergyElectronOf << energy << " " << x << " " << y << " " << z << std::endl;
			}
		}
	} else {
		//WTF
		throw "wtf";
	}

	G4Track *aTrack = step->GetTrack();
	aTrack->SetTrackStatus(fStopAndKill);
}

} /* namespace SoEiXRS */
