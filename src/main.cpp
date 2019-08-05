#include "G4RunManager.hh"

#include <iostream>

#include "include.hh"

#include "FTFP_BERT.hh"

using namespace SoEiXRS;

enum DetectorPosition {
	transmission, perpendicular
};

int main(int argc, char* argv[]) {

	if (argc != 7) {
		std::cout << "Expects 6 arguments: " << std::endl;
		std::cout << "  beam energy (in keV)" << std::endl;
		std::cout << "  beam energy fluctuations (in keV), gaussian distribution" << std::endl;
		std::cout << "  target thickness (in mm)" << std::endl;
		std::cout << "  target angle (in degrees, 0 means perpendicular)" << std::endl;
		std::cout << "  detector position (\"transmission\" or \"perpendicular\")" << std::endl;
		std::cout << "  detector opening angle (in degrees)" << std::endl;
		return 0;
	}

	double beamEnergy = std::stod(argv[1]);
	double beamEnergyFluc = std::stod(argv[2]);
	double targetThickness = std::stod(argv[3]);
	double targetAngle = std::stod(argv[4]);
	DetectorPosition detectorPosition;
	std::string s1(argv[5]);
	if (s1 == "transmission") {
		detectorPosition = transmission;
	} else if (s1 == "perpendicular") {
		detectorPosition = perpendicular;
	} else {
		std::cout << "unknown detector position" << std::endl;
		exit(0);
	}
	double detectorOpeningAngle = std::stod(argv[6]);

	std::cout << "#initial data:" << std::endl;
	std::cout << "# beam energy: " << beamEnergy << "keV" << std::endl;
	std::cout << "# beam energy width: " << beamEnergyFluc << "keV" << std::endl;
	std::cout << "# target thickness: " << targetThickness << "mm" << std::endl;
	std::cout << "# target angle: " << targetAngle << "°" << std::endl;
	std::cout << "# detector position: " << s1 << std::endl;
	std::cout << "# detector opening angle: " << detectorOpeningAngle << "°" << std::endl;

	// construct the default run manager
	G4RunManager* runManager = new G4RunManager();

	// set mandatory initialization classes
	G4VModularPhysicsList* physicsList = new FTFP_BERT;
	runManager->SetUserInitialization(physicsList);
	runManager->SetUserInitialization(
			new DetectorConstruction(targetThickness, targetAngle));
	runManager->SetUserInitialization(
			new ActionInitialization(beamEnergy, beamEnergyFluc));

	// initialize G4 kernel
	runManager->Initialize();

	UIThread uithread();

	// start a run
	int numberOfEvent = 1;//10000000;
	runManager->BeamOn(numberOfEvent);

	std::string s;
	std::cin >> s;

	// job termination
	delete runManager;

	std::cout << "done without gamma" << std::endl;

	return 0;
}
