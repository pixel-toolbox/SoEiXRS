#include "ActionInitialization.hpp"

#include "PrimaryGeneratorAction.hpp"
#include "SteppingAction.hpp"

namespace SoEiXRS {

ActionInitialization::ActionInitialization(double energy, double energyFluc,
		const char* allEnergyOutFile, const char* detectorEnergyOutFile,
		DetectorPosition detPos, double detectorAngle) :
		energy(energy), energyFluc(energyFluc), G4VUserActionInitialization(), allEnergyOutFile(
				allEnergyOutFile), detectorEnergyOutFile(detectorEnergyOutFile), detPos(
				detPos), detectorAngle(detectorAngle) {
}

void ActionInitialization::Build() const {
	SetUserAction(new PrimaryGeneratorAction(energy, energyFluc));

	SetUserAction(
			new SteppingAction(allEnergyOutFile, detectorEnergyOutFile, detPos,
					detectorAngle));
}

ActionInitialization::~ActionInitialization() {
}

} /* namespace SoEiXRS */
