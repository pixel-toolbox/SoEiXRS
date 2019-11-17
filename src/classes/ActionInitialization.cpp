#include "ActionInitialization.hpp"

#include "PrimaryGeneratorAction.hpp"
#include "SteppingAction.hpp"

namespace SoEiXRS {

ActionInitialization::ActionInitialization(double energy, double energyFluc,
		const char* detectorEnergyOutFile,
		const char* detectorEnergyElectronOutFile, DetectorPosition detPos,
		double detectorAngle) :
		energy(energy), energyFluc(energyFluc), G4VUserActionInitialization(), detectorEnergyOutFile(
				detectorEnergyOutFile), detectorEnergyElectronOutFile(
				detectorEnergyElectronOutFile), detPos(detPos), detectorAngle(
				detectorAngle) {
}

void ActionInitialization::Build() const {
	SetUserAction(new PrimaryGeneratorAction(energy, energyFluc));

	SetUserAction(
			new SteppingAction(detectorEnergyOutFile,
					detectorEnergyElectronOutFile, detPos, detectorAngle));
}

ActionInitialization::~ActionInitialization() {
}

} /* namespace SoEiXRS */
