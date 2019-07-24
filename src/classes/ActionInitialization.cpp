/*
 * ActionInitialization.cpp
 *
 *  Created on: 08.07.2019
 *      Author: simon
 */

#include "ActionInitialization.hpp"

#include "PrimaryGeneratorAction.hpp"
#include "SteppingAction.hpp"

namespace SoEiXRS {

ActionInitialization::ActionInitialization(double energy, double energyFluc) :
		energy(energy),
		energyFluc(energyFluc),
		G4VUserActionInitialization() {}

void ActionInitialization::Build() const {
	SetUserAction(new PrimaryGeneratorAction(energy, energyFluc));

	SetUserAction(new SteppingAction());
}

ActionInitialization::~ActionInitialization() {}

} /* namespace SoEiXRS */
