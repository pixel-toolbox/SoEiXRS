#include "UIThread.hpp"
#include "G4VisExecutive.hh"
#include "G4UImanager.hh"

#include <iostream>

namespace SoEiXRS {

UIThread::UIThread() :
		keepRunning(true), threadLoop(loop, &keepRunning) {
	std::cout << "starting UI thread" << std::endl;
}

UIThread::~UIThread() {
	keepRunning = false;
	threadLoop.join();
}

void UIThread::loop(std::atomic<bool>* keepRunning) {

	// get the pointer to the UI manager and set verbosities
	G4UImanager* UI = G4UImanager::GetUIpointer();
	UI->ApplyCommand("/run/verbose 0");
	UI->ApplyCommand("/event/verbose 0");
	UI->ApplyCommand("/tracking/verbose 0");

	G4VisManager* visManager = new G4VisExecutive;
	visManager->Initialise();

	UI->ApplyCommand("/control/execute vis.mac");

	while (keepRunning->load()) {
		UI->ApplyCommand("/vis/viewer/flush");
	}

	delete visManager;
}

} /* namespace SoEiXRS */
